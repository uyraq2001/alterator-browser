#include "model.h"

#include "../../aobuilder/builders/aobuilderimpl.h"
#include "../../aobuilder/constants.h"
#include "objectitem.h"

#include <QDebug>
#include <QStandardItemModel>
#include <qstandarditemmodel.h>

#include <algorithm>
#include <memory>
#include <set>
#include <variant>
#include <vector>

template<typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

namespace ab::model
{
Model::Model()
{
    QStandardItem *root     = this->invisibleRootItem();
    const auto modelBuilder = std::make_unique<ao_builder::AOBuilderImpl>();

    this->categoriesRoot = std::make_unique<ModelItem>();
    auto categories      = modelBuilder->buildCategories();
    for (auto &category : categories)
    {
        auto categoryItem = std::make_unique<ModelItem>(ModelItem::ItemType::Category, std::move(category));
        this->categoriesRoot->appendRow(categoryItem.release());
    }
    root->appendRow(this->categoriesRoot.get());

    this->appsRoot = std::make_unique<ModelItem>();
    auto apps      = modelBuilder->buildLocalApps();
    for (auto &app : apps)
    {
        auto appItem = std::make_unique<ModelItem>(ModelItem::ItemType::LocalApplication, std::move(app));
        this->appsRoot->appendRow(appItem.release());
    }
    root->appendRow(this->appsRoot.get());

    this->legacyObjectsRoot = std::make_unique<ModelItem>();
    auto legacyObjects      = modelBuilder->buildLegacyObjects();
    for (auto &legacyObject : legacyObjects)
    {
        auto objectItem = std::make_unique<ModelItem>(ModelItem::ItemType::LegacyObject, std::move(legacyObject));
        this->legacyObjectsRoot->appendRow(objectItem.release());
    }
    root->appendRow(this->legacyObjectsRoot.get());
}

Model::~Model() = default;

std::vector<ao_builder::Id> Model::getCategories()
{
    std::vector<ao_builder::Id> categoriesIds{};

    for (int i = 0; i < this->categoriesRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->categoriesRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item != nullptr)
        {
            categoriesIds.push_back(item->getObject()->m_id);
        }
    }
    return categoriesIds;
}

std::optional<ao_builder::Category> Model::getCategory(ao_builder::Id category_id)
{
    for (int i = 0; i < this->categoriesRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->categoriesRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto category              = dynamic_cast<ao_builder::Category *>(object);

        if (category != nullptr && category->m_id == category_id)
        {
            return {*category};
        }
    }

    return {};
}

std::vector<ao_builder::Id> Model::getLocalApplications()
{
    std::vector<ao_builder::Id> appsIds;

    for (int i = 0; i < this->appsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->appsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item != nullptr)
        {
            appsIds.push_back(item->getObject()->m_id);
        }
    }
    return appsIds;
}

std::optional<ao_builder::LocalAppObject> Model::getLocalApplication(ao_builder::Id appId)
{
    for (int i = 0; i < this->appsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->appsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto app                   = dynamic_cast<ao_builder::LocalAppObject *>(object);

        if (app != nullptr && app->m_id == appId)
        {
            return {*app};
        }
    }

    return {};
}

std::set<QString> Model::getInterfaces()
{
    std::set<QString> interfaces{};

    for (int i = 0; i < this->appsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->appsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto app                   = dynamic_cast<ao_builder::LocalAppObject *>(object);

        if (app != nullptr)
        {
            interfaces.insert(app->m_interfaces.begin(), app->m_interfaces.end());
        }
    }

    return interfaces;
}

std::vector<ao_builder::Id> Model::getLocalApplicationsByInterface(QString iface)
{
    std::vector<ao_builder::Id> appsIds{};

    for (int i = 0; i < this->appsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->appsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto app                   = dynamic_cast<ao_builder::LocalAppObject *>(object);

        if (app == nullptr)
        {
            continue;
        }

        const auto find = std::find(app->m_interfaces.begin(), app->m_interfaces.end(), iface);
        if (find != app->m_interfaces.end())
        {
            appsIds.push_back(item->getObject()->m_id);
        }
    }
    return appsIds;
}

std::vector<ao_builder::Id> Model::getLegacyObjects()
{
    std::vector<ao_builder::Id> legacyObjectsIds{};

    for (int i = 0; i < this->legacyObjectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->legacyObjectsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item != nullptr)
        {
            legacyObjectsIds.push_back(item->getObject()->m_id);
        }
    }
    return legacyObjectsIds;
}

std::optional<ao_builder::LegacyObject> Model::getLegacyObject(ao_builder::Id legacyObjectId)
{
    for (int i = 0; i < this->legacyObjectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->legacyObjectsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto legacyObject          = dynamic_cast<ao_builder::LegacyObject *>(object);

        if (legacyObject != nullptr && legacyObject->m_id == legacyObjectId)
        {
            return {*legacyObject};
        }
    }
    return {};
}

std::vector<ao_builder::Id> Model::getLegacyObjectsByInterface(QString iface)
{
    return iface == ao_builder::DBUS_LEGACY_OBJECTS_INTERFACE_NAME ? this->getLegacyObjects()
                                                                   : std::vector<ao_builder::Id>();
}

std::vector<ao_builder::Id> Model::getLegacyObjectsByCategory(ao_builder::Id category_id)
{
    std::vector<ao_builder::Id> legacyObjectIds{};

    for (int i = 0; i < this->legacyObjectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->legacyObjectsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();
        auto legacyObject          = dynamic_cast<ao_builder::LegacyObject *>(object);

        if (legacyObject != nullptr && legacyObject->m_categoryId == category_id)
        {
            legacyObjectIds.push_back(legacyObject->m_id);
        }
    }
    return legacyObjectIds;
}

void Model::translateModel(QString locale)
{
    translateItem(invisibleRootItem(), locale);
}

void Model::translateItem(QStandardItem *item, QString locale)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = item->child(i);
        auto currentItem                   = dynamic_cast<ModelItem *>(currentStandardItem);
        if (!currentItem)
        {
            qWarning() << "Can't cast item to ObjectItem to translate";
            continue;
        }

        if (currentItem->rowCount() > 0)
        {
            translateItem(currentItem, locale);
        }

        currentItem->getObject()->setLocale(locale);
    }
}
} // namespace ab::model
