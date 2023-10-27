#include "objectsmodelbuilder.h"
#include "category.h"
#include "categorybuilder.h"
#include "constants.h"
#include "desktopfileparser.h"
#include "localapplicationmodel.h"
#include "model/localapllicationmodelbuilder.h"
#include "model/model.h"
#include "object.h"
#include "objectbuilderfactory.h"
#include "objectitem.h"

#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

class Semaphore
{
public:
    explicit Semaphore(size_t count)
        : m_count(count)
    {}

    void lock()
    { // call before critical section
        std::unique_lock<std::mutex> lock(mutex);
        condition_variable.wait(lock, [this] { return (m_count > 0); });
        --m_count;
    }

    void unlock()
    { // call after critical section
        std::unique_lock<std::mutex> lock(mutex);
        ++m_count;
        condition_variable.notify_one();
    }

private:
    std::mutex mutex{};
    std::condition_variable condition_variable{};
    size_t m_count;
};

template<typename T>
static bool isReady(std::future<T> &f)
{
    return f.valid() && f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

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
    : m_dbusServiceName(std::move(serviceName))
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
        qCritical() << "Local applications model is empty";
    }

    QStringList pathsOfObjects = getListOfObjects();

    if (pathsOfObjects.isEmpty())
    {
        return std::make_unique<Model>();
    }

    std::vector<std::variant<Object, Category>> acObjects = parseObjects(pathsOfObjects);

    if (acObjects.empty())
    {
        qCritical() << "Can't access alterator manager interface";
        return std::make_unique<Model>();
    }

    std::unique_ptr<Model> model = buildModelFromObjects(std::move(acObjects));

    mergeApplicationModel(model.get(), appModel.release());

    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    model->translateModel(language);

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
            qWarning() << "Can't cast category item to ObjectItem to merge models";
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
            qWarning() << "Can't cast item to ObjectItem to merge application object";
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
            qCritical() << "Item is not of Object type";
        }
    }
}

QStringList ObjectsModelBuilder::getListOfObjects()
{
    QDBusInterface managerIface(m_dbusServiceName, m_dbusPath, m_managerInterface, m_dbusConnection);

    if (!managerIface.isValid())
    {
        qCritical() << "Can't access alterator manager interface";
        return {};
    }

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface.call(m_getObjectMethodName, m_dbusFindInterface);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface:" << reply.error().message();
        return {};
    }

    QList<QDBusObjectPath> pathList = reply.value();

    QStringList paths;

    std::for_each(pathList.begin(), pathList.end(), [&paths](QDBusObjectPath &path) { paths.append(path.path()); });

    return paths;
}

std::vector<std::variant<Object, Category>> ObjectsModelBuilder::parseObjects(QStringList &pathsList)
{
    std::vector<std::future<std::variant<Object, Category>>> futureObjects;
    Semaphore maxConcurrentJobs(std::min(10u, std::thread::hardware_concurrency()));

    std::vector<std::variant<Object, Category>> objects;

    for (const QString &currentPath : pathsList)
    {
        futureObjects.push_back(std::async(
            std::launch::async,
            [this, currentPath](Semaphore &maxJobs) -> std::variant<Object, Category> {
                maxJobs.lock();
                QDBusInterface iface(m_dbusServiceName, currentPath, m_dbusFindInterface, m_dbusConnection);
                if (!iface.isValid())
                {
                    qWarning() << "Object" << currentPath << "does not provide interface" << m_dbusFindInterface;
                    return {};
                }

                QString currentObjectInfo = getObjectInfo(iface);
                if (currentObjectInfo.isEmpty())
                {
                    qWarning() << "Can't get info of object" << currentPath << "in interface" << m_dbusFindInterface;
                    return {};
                }

                DesktopFileParser infoParsingResult(currentObjectInfo);

                auto objectBuilder = ObjectBuilderFactory::getBuilder(&infoParsingResult);

                if (!objectBuilder)
                {
                    qWarning() << "Bad info format in object" << currentPath << "in interface" << m_dbusFindInterface;
                    return {};
                }

                auto newObject = std::variant<Object, Category>(
                    *(objectBuilder->buildObject(&infoParsingResult).release()));
                maxJobs.unlock();

                return newObject;
            },
            std::ref(maxConcurrentJobs)));
    }

    do
    {
        for (auto &future : futureObjects)
        {
            if (isReady(future))
            {
                objects.push_back(future.get());
            }
        }
    } while (objects.size() < futureObjects.size());

    return objects;
}

QString ObjectsModelBuilder::getObjectInfo(QDBusInterface &iface)
{
    QDBusReply<QByteArray> reply = iface.call(m_infoMethodName);

    if (!reply.isValid())
    {
        // TODO(mchernigin): log error
        return {};
    }

    return QString{reply.value()};
}

std::unique_ptr<Model> ObjectsModelBuilder::buildModelFromObjects(std::vector<std::variant<Object, Category>> objects)
{
    std::map<QString, std::unique_ptr<ObjectItem>> categories;

    for (auto &object : objects)
    {
        Object currentObject = std::get<Object>(object);

        auto find = categories.find(currentObject.m_categoryId);

        if (find == categories.end())
        {
            auto newCategoryItem = createCategoryItem(currentObject.m_categoryId);

            auto newModuleItem        = std::make_unique<ObjectItem>();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object   = std::make_unique<std::variant<Object, Category>>(std::move(object));

            newCategoryItem->appendRow(newModuleItem.release());

            categories[currentObject.m_categoryId] = std::move(newCategoryItem);
        }
        else
        {
            auto categoryItem = &find->second;

            auto newModuleItem        = std::make_unique<ObjectItem>();
            newModuleItem->m_itemType = ObjectItem::ItemType::module;
            newModuleItem->m_object   = std::make_unique<std::variant<Object, Category>>(std::move(object));

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
    auto newCategoryItem               = std::make_unique<ObjectItem>();
    std::unique_ptr<Category> category = nullptr;

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
        qWarning() << "Can not connect to interface" << CATEGORY_INTERFACE_NAME_FOR_ACOBJECT;
    }

    QDBusReply<QByteArray> reply = iface->call(CATEGORY_METHOD_NAME_FOR_ACOBJECT, categoryName);

    if (!reply.isValid())
    {
        qWarning() << "Can't reply with category name for the category" << categoryName;
    }
    else
    {
        QString categoryData(reply.value());
        DesktopFileParser categoryParser(categoryData);
        ObjectCategoryBuilder categoryBuilder(&categoryParser);
        category = categoryBuilder.buildObjectCategory();
    }

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
    newCategoryItem->m_object   = std::make_unique<std::variant<Object, Category>>(*category);

    return newCategoryItem;
}
} // namespace model
} // namespace ab
