#include "objectsmodelbuilder.h"
#include "desktopfileparser.h"
#include "localapplicationmodel.h"
#include "model/model.h"
#include "objectbuilder.h"
#include "objectitem.h"

#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

namespace ab
{
namespace model
{
ObjectsModelBuilder::ObjectsModelBuilder(QString serviceName,
                                         QString dbusPath,
                                         QString managerIface,
                                         QString findInterface,
                                         QString getObjectMethodName,
                                         QString infoMethodName,
                                         QString categoryInterfaceName,
                                         QString categoryMethodName)
    : m_dbusConnection(QDBusConnection::systemBus())
    , m_dbusServiceName(serviceName)
    , m_dbusPath(dbusPath)
    , m_managerInterface(managerIface)
    , m_dbusFindInterface(findInterface)
    , m_getObjectMethodName(getObjectMethodName)
    , m_infoMethodName(infoMethodName)
    , m_categoryInterfaceName(categoryInterfaceName)
    , m_categoryMethodName(categoryMethodName)
{}

std::unique_ptr<Model> ObjectsModelBuilder::buildModel(LocalApplicationModel *appModel)
{
    if (!appModel)
    {
        qCritical() << "Local applications model is empty!!";
    }

    QStringList pathsOfObjects = getListOfObjects();

    if (pathsOfObjects.isEmpty())
    {
        return std::make_unique<Model>();
    }

    std::vector<std::unique_ptr<Object>> acObjects = parseObjects(pathsOfObjects);

    if (acObjects.empty())
    {
        qCritical() << "Can't access alterator manager interface!";
        return std::make_unique<Model>();
    }

    std::unique_ptr<Model> model = buildModelFromObjects(std::move(acObjects));

    mergeApplicationModel(model.get(), appModel);

    return model;
}

void ObjectsModelBuilder::mergeApplicationModel(Model *objectModel, LocalApplicationModel *appModel)
{
    QStandardItem *rootItem = objectModel->invisibleRootItem();

    int g = rootItem->rowCount();

    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = rootItem->child(i);
        ObjectItem *currentCategoryItem    = dynamic_cast<ObjectItem *>(currentStandardItem);
        if (!currentCategoryItem)
        {
            qWarning() << "Can't cast category item to ObjectItem to merge models!";

            continue;
        }

        mergeObjectWithApp(currentCategoryItem, appModel);
    }
}

void ObjectsModelBuilder::mergeObjectWithApp(ObjectItem *item, LocalApplicationModel *appModel)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = item->child(i);
        ObjectItem *currentModuleItem      = dynamic_cast<ObjectItem *>(currentStandardItem);
        if (!currentModuleItem)
        {
            qWarning() << "Can't cast item to ObjectItem to merge application object!";
            continue;
        }

        if (currentModuleItem->rowCount() > 0)
        {
            mergeObjectWithApp(currentModuleItem, appModel);
        }

        if (!currentModuleItem->getObject()->m_interfaces.empty())
        {
            for (QString &currentIface : currentModuleItem->getObject()->m_interfaces)
            {
                std::vector<LocalApplication *> apps = appModel->getAppsByInterface(currentIface);

                std::for_each(apps.begin(), apps.end(), [currentModuleItem](LocalApplication *app) {
                    currentModuleItem->getObject()->m_applications.push_back(app);
                });
            }
        }
    }
}

QStringList ObjectsModelBuilder::getListOfObjects()
{
    QDBusInterface managerIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!managerIface.isValid())
    {
        qCritical() << "Can't access alterator manager interface!";
        return QStringList();
    }

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface.call(m_getObjectMethodName, m_dbusFindInterface);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface!";
        return QStringList();
    }

    QList<QDBusObjectPath> pathList = reply.value();

    QStringList paths;

    std::for_each(pathList.begin(), pathList.end(), [&paths](QDBusObjectPath &path) { paths.append(path.path()); });

    return paths;
}

std::vector<std::unique_ptr<Object>> ObjectsModelBuilder::parseObjects(QStringList &pathsList)
{
    std::vector<std::unique_ptr<Object>> acObjects;

    QDBusInterface categoryIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!categoryIface.isValid())
    {
        qCritical() << "can't find interface to find object with categories interface";

        return acObjects;
    }

    QDBusReply<QList<QDBusObjectPath>> reply = categoryIface.call(m_getObjectMethodName, m_categoryInterfaceName);

    if (!reply.isValid())
    {
        qCritical() << "Reply is invalid. Can't find find interface to find object with categories interface";

        return acObjects;
    }

    // TODO: check for empty QList
    QString categoryObjectPath = reply.value().at(0).path();

    QDBusInterface categoryInfoIface(m_dbusServiceName, categoryObjectPath, m_categoryInterfaceName, m_dbusConnection);

    if (!categoryIface.isValid())
    {
        qCritical() << "can't connect to category object!";

        return acObjects;
    }

    for (QString &currentPath : pathsList)
    {
        QDBusInterface iface(m_dbusServiceName, currentPath, m_dbusFindInterface, m_dbusConnection);

        if (!iface.isValid())
        {
            qWarning() << "Warning: object: " + currentPath + " doesn't provide interface " + m_dbusFindInterface;

            continue;
        }

        QString currentObjectInfo = getObjectInfo(iface);

        if (currentObjectInfo.isEmpty())
        {
            qWarning() << "Warning: Can't get info of object: " + currentPath + " in interface: " + m_dbusFindInterface;

            continue;
        }

        DesktopFileParser infoParsingResult(currentObjectInfo);

        ObjectBuilder objectBuilder(&infoParsingResult, &categoryInfoIface, m_categoryMethodName);

        std::unique_ptr<Object> newObject = objectBuilder.buildObject();

        if (newObject)
        {
            acObjects.push_back(std::move(newObject));
        }
    }

    return acObjects;
}

QString ObjectsModelBuilder::getObjectInfo(QDBusInterface &iface)
{
    QDBusReply<QByteArray> reply = iface.call(m_infoMethodName);

    if (!reply.isValid())
    {
        return QString();
    }

    QString result = QString(reply.value());

    return result;
}

std::unique_ptr<Model> ObjectsModelBuilder::buildModelFromObjects(std::vector<std::unique_ptr<Object>> objects)
{
    auto model = std::make_unique<Model>();

    QMap<QString, ObjectItem *> categories;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        Object *currentObject = objects.at(i).get();

        auto it = categories.find(currentObject->m_displayCategory);

        if (it == categories.end())
        {
            ObjectItem *newCategoryItem = createCategoryItem(currentObject->m_displayCategory,
                                                             currentObject->m_categoryObject.get());

            categories[newCategoryItem->getObject()->m_displayCategory] = newCategoryItem;

            model->appendRow(newCategoryItem);

            ObjectItem *newModuleItem = new ObjectItem();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object = std::move(objects.at(i));

            newCategoryItem->appendRow(newModuleItem);
        }
        else
        {
            ObjectItem *categoryItem = *it;

            ObjectItem *newModuleItem = new ObjectItem();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object = std::move(objects.at(i));

            categoryItem->appendRow(newModuleItem);
        }
    }
    return model;
}

ObjectItem *ObjectsModelBuilder::createCategoryItem(QString, ObjectCategory *nameTranslations)
{
    ObjectItem *newCategoryItem = new ObjectItem();

    newCategoryItem->m_itemType = ObjectItem::ItemType::category;

    Object *newObject = newCategoryItem->getObject();

    ObjectCategory *newObjectCategory = newObject->m_categoryObject.get();

    for (QString currentKey : nameTranslations->m_nameLocaleStorage.keys())
    {
        newObjectCategory->m_nameLocaleStorage.insert(currentKey, nameTranslations->m_nameLocaleStorage[currentKey]);
    }

    for (QString currentKey : nameTranslations->m_commentLocaleStorage.keys())
    {
        newObjectCategory->m_commentLocaleStorage.insert(currentKey,
                                                           nameTranslations->m_commentLocaleStorage[currentKey]);
    }

    newObjectCategory->m_id = nameTranslations->m_id;
    newObject->m_displayCategory = nameTranslations->m_id;
    newObjectCategory->m_name = nameTranslations->m_name;
    newObjectCategory->m_comment = nameTranslations->m_comment;
    newObjectCategory->m_icon = nameTranslations->m_icon;
    newObjectCategory->m_type = nameTranslations->m_type;
    newObjectCategory->m_xAlteratorCategory = nameTranslations->m_xAlteratorCategory;

    return newCategoryItem;
}
} // namespace model
} // namespace ab
