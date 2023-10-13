#include "objectsmodelbuilder.h"
#include "category.h"
#include "categorybuilder.h"
#include "desktopfileparser.h"
#include "localapplicationmodel.h"
#include "model/localapllicationmodelbuilder.h"
#include "model/model.h"
#include "object.h"
#include "objectbuilder.h"
#include "objectitem.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

const QString DBUS_SERVICE_NAME                    = "ru.basealt.alterator";
const QString DBUS_PATH                            = "/ru/basealt/alterator";
const QString DBUS_FIND_INTERFACE_NAME             = "ru.basealt.alterator.object";
const QString DBUS_MANAGER_INTERFACE_NAME          = "ru.basealt.alterator.manager";
const QString GET_OBJECTS_METHOD_NAME              = "get_objects";
const QString INFO_METHOD_NAME_FOR_ACOBJECT        = "info";
const QString CATEGORY_INTERFACE_NAME_FOR_ACOBJECT = "ru.basealt.alterator.categories";
const QString CATEGORY_OBJECT_PATH                 = "/ru/basealt/alterator/categories";
const QString CATEGORY_METHOD_NAME_FOR_ACOBJECT    = "info";

const QString DBUS_LOCAL_APP_PATH              = "/ru/basealt/alterator/applications";
const QString DBUS_LOCAL_APP_INTERFACE_NAME    = "ru.basealt.alterator.applications";
const QString DBUS_LOCAL_APP_GET_LIST_OF_FILES = "list";
const QString DBUS_LOCAL_APP_GET_DESKTOP_FILE  = "info";

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
                                         QString categoryMethodName,
                                         QString interfaceName,
                                         QString getListOfFilesMethod,
                                         QString getDesktopFileMethod)
    : m_dbusConnection(QDBusConnection::systemBus())
    , m_dbusServiceName(std::move(serviceName))
    , m_dbusPath(std::move(dbusPath))
    , m_managerInterface(std::move(managerIface))
    , m_dbusFindInterface(std::move(findInterface))
    , m_getObjectMethodName(std::move(getObjectMethodName))
    , m_infoMethodName(std::move(infoMethodName))
    , m_categoryInterfaceName(std::move(categoryInterfaceName))
    , m_categoryMethodName(std::move(categoryMethodName))
    , m_interface(std::move(interfaceName))
    , m_getFilesMethodName(std::move(getListOfFilesMethod))
    , m_getDesktopFileMethodName(std::move(getDesktopFileMethod))
{}

std::unique_ptr<Model> ObjectsModelBuilder::buildModel()
{
    model::LocalApllicationModelBuilder appModelBuilder(DBUS_SERVICE_NAME,
                                                        DBUS_LOCAL_APP_PATH,
                                                        DBUS_LOCAL_APP_INTERFACE_NAME,
                                                        DBUS_LOCAL_APP_GET_LIST_OF_FILES,
                                                        DBUS_LOCAL_APP_GET_DESKTOP_FILE);

    std::unique_ptr<model::LocalApplicationModel> appModel = appModelBuilder.buildModel();

    if (!appModel)
    {
        qCritical() << "Local applications model is empty!!";
    }

    QStringList pathsOfObjects = getListOfObjects();

    if (pathsOfObjects.isEmpty())
    {
        return std::make_unique<Model>();
    }

    std::vector<std::unique_ptr<std::variant<Object, Category>>> acObjects = parseObjects(pathsOfObjects);

    if (acObjects.empty())
    {
        qCritical() << "Can't access alterator manager interface!";
        return std::make_unique<Model>();
    }

    std::unique_ptr<Model> model = buildModelFromObjects(std::move(acObjects));

    mergeApplicationModel(model.get(), appModel.release());

    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    model->translateModel("ru");

    return model;
}

void ObjectsModelBuilder::mergeApplicationModel(Model *objectModel, LocalApplicationModel *appModel)
{
    QStandardItem *rootItem = objectModel->invisibleRootItem();

    int g = rootItem->rowCount();

    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = rootItem->child(i);
        auto currentCategoryItem           = dynamic_cast<ObjectItem *>(currentStandardItem);
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
        auto currentModuleItem             = dynamic_cast<ObjectItem *>(currentStandardItem);
        if (!currentModuleItem)
        {
            qWarning() << "Can't cast item to ObjectItem to merge application object!";
            continue;
        }

        if (currentModuleItem->rowCount() > 0)
        {
            mergeObjectWithApp(currentModuleItem, appModel);
        }

        try
        {
            auto interfaces = std::get<ab::model::Object>(*currentModuleItem->getObject()).m_interfaces;
            if (!interfaces.empty())
            {
                for (std::size_t j = 0; j < interfaces.size(); j++)
                {
                    QString currentIface                 = interfaces.at(j);
                    std::vector<LocalApplication *> apps = appModel->getAppsByInterface(currentIface);

                    std::for_each(apps.begin(), apps.end(), [currentModuleItem](LocalApplication *app) {
                        std::get<ab::model::Object>(*currentModuleItem->getObject()).m_applications.push_back(app);
                    });
                }
            }
        }
        catch (const std::bad_variant_access &e)
        {
            qCritical() << "ERROR: the item is not of Object type";
        }
    }
}

QStringList ObjectsModelBuilder::getListOfObjects()
{
    QDBusInterface managerIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!managerIface.isValid())
    {
        qCritical() << "Can't access alterator manager interface!";
        return {};
    }

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface.call(m_getObjectMethodName, m_dbusFindInterface);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface!";
        return {};
    }

    QList<QDBusObjectPath> pathList = reply.value();

    QStringList paths;

    std::for_each(pathList.begin(), pathList.end(), [&paths](QDBusObjectPath &path) { paths.append(path.path()); });

    return paths;
}

std::vector<std::unique_ptr<std::variant<Object, Category>>> ObjectsModelBuilder::parseObjects(QStringList &pathsList)
{
    std::vector<std::unique_ptr<std::variant<Object, Category>>> acObjects;

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

        ObjectBuilder objectBuilder(&infoParsingResult);

        std::unique_ptr<std::variant<Object, Category>> newObject = std::make_unique<std::variant<Object, Category>>(
            *(objectBuilder.buildObject()));

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
        return {};
    }

    QString result = QString(reply.value());

    return result;
}

std::unique_ptr<Model> ObjectsModelBuilder::buildModelFromObjects(
    std::vector<std::unique_ptr<std::variant<Object, Category>>> objects)
{
    std::map<QString, std::unique_ptr<ObjectItem>> categories;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        Object currentObject = std::get<Object>(*objects.at(i));

        auto find = categories.find(currentObject.m_categoryId);

        if (find == categories.end())
        {
            auto newCategoryItem = createCategoryItem(currentObject.m_categoryId);

            auto newModuleItem        = std::make_unique<ObjectItem>();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object   = std::move(objects.at(i));

            newCategoryItem->appendRow(newModuleItem.release());

            categories[currentObject.m_categoryId] = std::move(newCategoryItem);
        }
        else
        {
            auto categoryItem = &find->second;

            auto newModuleItem        = std::make_unique<ObjectItem>();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object   = std::move(objects.at(i));

            categoryItem->get()->appendRow(newModuleItem.release());
        }
    }

    auto model = std::make_unique<Model>();
    for (auto &category : categories)
    {
        model->appendRow(category.second.release());
    }
    return model;
}

std::unique_ptr<ObjectItem> ObjectsModelBuilder::createCategoryItem(QString categoryName)
{
    auto newCategoryItem = std::make_unique<ObjectItem>();

    if (categoryName.isEmpty())
    {
        categoryName = "Miscellaneous";
    }

    std::unique_ptr<QDBusInterface> iface = std::make_unique<QDBusInterface>(DBUS_SERVICE_NAME,
                                                                             CATEGORY_OBJECT_PATH,
                                                                             CATEGORY_INTERFACE_NAME_FOR_ACOBJECT,
                                                                             QDBusConnection::systemBus());
    if (!iface->isValid())
    {
        qWarning() << "Can not connect to " + CATEGORY_INTERFACE_NAME_FOR_ACOBJECT + " interface";
        return newCategoryItem;
    }
    QDBusReply<QByteArray> reply = iface->call(CATEGORY_METHOD_NAME_FOR_ACOBJECT, categoryName);

    if (!reply.isValid())
    {
        qWarning() << "Can't reply with category name for the category:" << categoryName;
        return newCategoryItem;
    }

    QString categoryData(reply.value());

    DesktopFileParser categoryParser(categoryData);

    ObjectCategoryBuilder categoryBuilder(&categoryParser);

    std::unique_ptr<Category> category = categoryBuilder.buildObjectCategory();

    if (!category)
    {
        category                                  = std::make_unique<Category>();
        category->m_id                            = "Unknown";
        category->m_name                          = "Unknown";
        category->m_comment                       = "Unable to get category";
        category->m_icon                          = "groups/system";
        category->m_type                          = "Directory";
        category->m_xAlteratorCategory            = "X-Alterator-Unknown";
        category->m_nameLocaleStorage["ru_RU"]    = "Без категории";
        category->m_commentLocaleStorage["ru_RU"] = "Ошибка при получении категории";
    }

    newCategoryItem->m_itemType = ObjectItem::ItemType::category;
    newCategoryItem->m_object   = std::move(std::make_unique<std::variant<Object, Category>>(*category));

    return newCategoryItem;
}
} // namespace model
} // namespace ab
