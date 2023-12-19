#include "model.h"

#include "../../aobuilder/builders/aobuilderimpl.h"
#include "../../aobuilder/constants.h"
#include "model/item.h"

#include <QDebug>
#include <QStandardItemModel>
#include <qstandarditemmodel.h>

#include <algorithm>
#include <memory>
#include <set>
#include <variant>
#include <vector>

namespace ab::model
{
Model::Model()
{
    QStandardItem *root = this->invisibleRootItem();

    root->appendRow(this->categoriesRoot.get());
    root->appendRow(this->appsRoot.get());
    root->appendRow(this->objectsRoot.get());
}

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

    for (int i = 0; i < this->objectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->objectsRoot->child(i);
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
    for (int i = 0; i < this->objectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->objectsRoot->child(i);
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

    for (int i = 0; i < this->objectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->objectsRoot->child(i);
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

        if (currentItem->m_itemType != ab::model::ModelItem::ItemType::None)
        {
            currentItem->getObject()->setLocale(locale);
        }
    }
}

void Model::build(std::vector<std::unique_ptr<ao_builder::Object>> categories,
                  std::vector<std::unique_ptr<ao_builder::Object>> apps,
                  std::vector<std::unique_ptr<ao_builder::Object>> objects)
{
    for (auto &category : categories)
    {
        auto categoryItem = std::make_unique<ModelItem>(ModelItem::ItemType::Category, std::move(category));
        this->categoriesRoot->appendRow(categoryItem.release());
    }

    for (auto &app : apps)
    {
        auto appItem = std::make_unique<ModelItem>(ModelItem::ItemType::LocalApplication, std::move(app));
        this->appsRoot->appendRow(appItem.release());
    }

    for (auto &object : objects)
    {
        const auto type = object->m_isLegacy ? ModelItem::ItemType::LegacyObject : ModelItem::ItemType::Object;
        auto objectItem = std::make_unique<ModelItem>(type, std::move(object));
        this->objectsRoot->appendRow(objectItem.release());
    }
}
} // namespace ab::model
