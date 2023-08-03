#include "acobjectsmodelbuilder.h"
#include "aclocalapplicationmodel.h"
#include "acobjectbuilder.h"
#include "acobjectitem.h"
#include "desktopfileparser.h"
#include "acobjectcategorybuilder.h"
#include "model/acmodel.h"

#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <qdbusreply.h>

ACObjectsModelBuilder::ACObjectsModelBuilder(QString serviceName,
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

std::unique_ptr<ACModel> ACObjectsModelBuilder::buildModel(ACLocalApplicationModel *appModel)
{
    if (!appModel)
    {
        qWarning() << "ERROR! Local applications model is empty!!";

        return std::unique_ptr<ACModel>(nullptr);
    }

    QStringList pathsOfACObjects = getListOfACObjects();

    if (pathsOfACObjects.isEmpty())
    {
        return std::unique_ptr<ACModel>(nullptr);
    }

    std::vector<std::unique_ptr<ACObject>> acObjects = parseACObjects(pathsOfACObjects);

    if (acObjects.empty())
    {
        qWarning() << "ERROR! Can't access alterator manager interface!";

        return std::unique_ptr<ACModel>(nullptr);
    }

    std::unique_ptr<ACModel> model = buildModelFromACObjects(std::move(acObjects));

//    mergeApplicationModel(model.get(), appModel);

    return model;
}

//void ACObjectsModelBuilder::mergeApplicationModel(ACModel *objectModel, ACLocalApplicationModel *appModel)
//{
//    QStandardItem *rootItem = objectModel->invisibleRootItem();

//    int g = rootItem->rowCount();

//    for (int i = 0; i < rootItem->rowCount(); ++i)
//    {
//        QStandardItem *currentStandardItem = rootItem->child(i);
//        ACObjectItem *currentCategoryItem  = dynamic_cast<ACObjectItem *>(currentStandardItem);
//        if (!currentCategoryItem)
//        {
//            qWarning() << "WARNING! Can't cast category item to ACObjectItem to merge models!";

//            continue;
//        }

//        mergeObjectWithApp(currentCategoryItem, appModel);
//    }
//}

//void ACObjectsModelBuilder::mergeObjectWithApp(ACObjectItem *item, ACLocalApplicationModel *appModel)
//{
//    for (int i = 0; i < item->rowCount(); ++i)
//    {
//        QStandardItem *currentStandardItem = item->child(i);
//        ACObjectItem *currentModuleItem    = dynamic_cast<ACObjectItem *>(currentStandardItem);
//        if (!currentModuleItem)
//        {
//            qWarning() << "WARNING! Can't cast item to ACObjectItem to merge application object!";

//            continue;
//        }

//        if (currentModuleItem->rowCount() > 0)
//        {
//            mergeObjectWithApp(currentModuleItem, appModel);
//        }

//        if (!currentModuleItem->getACObject()->m_interfaces.empty())
//        {
//            for (QString &currentIface : currentModuleItem->getACObject()->m_interfaces)
//            {
//                std::vector<ACLocalApplication *> apps = appModel->getAppsByInterface(currentIface);

//                std::for_each(apps.begin(), apps.end(), [currentModuleItem](ACLocalApplication *app) {
//                    currentModuleItem->getACObject()->m_applications.push_back(app);
//                });
//            }
//        }
//    }
//}

QStringList ACObjectsModelBuilder::getListOfACObjects()
{
    QDBusInterface managerIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!managerIface.isValid())
    {
        qWarning() << "ERROR! Can't access alterator manager interface!";

        return QStringList();
    }

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface.call(m_getObjectMethodName, m_dbusFindInterface);

    if (!reply.isValid())
    {
        qWarning() << "ERROR! Can't get reply from alterator manager interface!";

        return QStringList();
    }

    QList<QDBusObjectPath> pathList = reply.value();

    QStringList paths;

    std::for_each(pathList.begin(), pathList.end(), [&paths](QDBusObjectPath &path) { paths.append(path.path()); });

    return paths;
}

std::vector<std::unique_ptr<ACObject>> ACObjectsModelBuilder::parseACObjects(QStringList &pathsList)
{
    std::vector<std::unique_ptr<ACObject>> acObjects;

    QDBusInterface categoryIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!categoryIface.isValid())
    {
        qWarning() << "ERROR: can't find interface to find object with categories interface";

        return acObjects;
    }

    QDBusReply<QList<QDBusObjectPath>> reply = categoryIface.call(m_getObjectMethodName, m_categoryInterfaceName);

    if (!reply.isValid())
    {
        qWarning() << "ERROR: Reply is invalid. Can't find find interface to find object with categories interface";

        return acObjects;
    }

    //TO DO check for empty QList
    QString categoryObjectPath = reply.value().at(0).path();

    QDBusInterface categoryInfoIface(m_dbusServiceName, categoryObjectPath, m_categoryInterfaceName, m_dbusConnection);

    if (!categoryIface.isValid())
    {
        qWarning() << "ERROR: can't connect to category object!";

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

        ACObjectBuilder objectBuilder(&infoParsingResult, &categoryInfoIface, m_categoryMethodName);

        std::unique_ptr<ACObject> newObject = objectBuilder.buildACObject();

        if (newObject)
        {
            acObjects.push_back(std::move(newObject));
        }
    }

    return acObjects;
}

QString ACObjectsModelBuilder::getObjectInfo(QDBusInterface &iface)
{
    QDBusReply<QByteArray> reply = iface.call(m_infoMethodName);

    if (!reply.isValid())
    {
        return QString();
    }

    QString result = QString(reply.value());

    return result;
}

std::unique_ptr<ACModel> ACObjectsModelBuilder::buildModelFromACObjects(std::vector<std::unique_ptr<ACObject>> objects)
{
    std::unique_ptr<ACModel> model(new ACModel());

    QMap<QString, ACObjectItem *> categories;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        ACObject *currentObject = objects.at(i).get();

        auto it = categories.find(currentObject->m_categoryId);

        if (it == categories.end())
        {
            QDBusInterface managerIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);
            if (!managerIface.isValid()){
                qWarning() << "ERROR! Can't access alterator manager interface!";
                return std::unique_ptr<ACModel>(nullptr);
            }
            QDBusReply<QList<QDBusObjectPath>> catReply = managerIface.call(m_getObjectMethodName, m_categoryInterfaceName);
            if (!catReply.isValid()){
                qWarning() << "Can't get categoryies object!";
                return std::unique_ptr<ACModel>(nullptr);
            }
            QString categoryObjectPath = catReply.value().at(0).path();

            QDBusInterface categoryInfoIface(m_dbusServiceName, categoryObjectPath, m_categoryInterfaceName, m_dbusConnection);
            if (!categoryInfoIface.isValid()){
                qWarning() << "ERROR: can't connect to category object!";
                return std::unique_ptr<ACModel>(nullptr);
            }
            QDBusReply<QByteArray> reply = categoryInfoIface.call("info", currentObject->m_categoryId);
            if (!reply.isValid())
            {
                qWarning() << "WARNING! Can't reply with category name for the object: " << currentObject->m_categoryId;
                return std::unique_ptr<ACModel>(nullptr);
            }

            QString categoryData(reply.value());

            DesktopFileParser categoryParser(categoryData);

            ACObjectCategoryBuilder categoryBuilder(&categoryParser);

            std::unique_ptr<ACCategory> category = categoryBuilder.buildACObjectCategory();

            if (!category)
            {
                return std::unique_ptr<ACModel>(nullptr);
            }

            ACObjectItem *newCategoryItem = createCategoryItem(currentObject->m_categoryId, std::move(category));

            model->appendRow(newCategoryItem);

            categories.insert(currentObject->m_categoryId, newCategoryItem);

            ACObjectItem *newModuleItem = new ACObjectItem();
            newModuleItem->m_itemType   = ACObjectItem::ITEM_TYPE::module;
            newModuleItem->m_acEntity   = std::move(objects.at(i));

            newCategoryItem->appendRow(newModuleItem);
        }
        else
        {
            ACObjectItem *categoryItem = *it;

            ACObjectItem *newModuleItem = new ACObjectItem();
            newModuleItem->m_itemType   = ACObjectItem::ITEM_TYPE::module;
            newModuleItem->m_acEntity   = std::move(objects.at(i));

            categoryItem->appendRow(newModuleItem);
        }
    }
    return model;
}

ACObjectItem *ACObjectsModelBuilder::createCategoryItem(QString name, std::unique_ptr<ACCategory> nameTranslations)
{
    ACObjectItem *newCategoryItem = new ACObjectItem();

    newCategoryItem->m_itemType = ACObjectItem::ITEM_TYPE::category;

    newCategoryItem->setEntity(std::move(nameTranslations));

    return newCategoryItem;
}
