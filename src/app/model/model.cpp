#include "model.h"

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
    this->clear();
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

ao_builder::Category *Model::getCategory(ao_builder::Id category_id)
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
            return category;
        }
    }

    qWarning() << QString("There is no '%1' category in the model").arg(category_id);
    return {};
}

ao_builder::Category *Model::getDefaultCategory()
{
    return getCategory(ao_builder::DEFAULT_CATEGORY_NAME);
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

ao_builder::LocalAppObject *Model::getLocalApplication(ao_builder::Id appId)
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
            return app;
        }
    }

    qWarning() << QString("There is no '%1' application in the model").arg(appId);
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

std::vector<ao_builder::Id> Model::getObjects()
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

ao_builder::Object *Model::getObject(ao_builder::Id objectId)
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

        if (object != nullptr && object->m_id == objectId)
        {
            return object;
        }
    }

    qWarning() << QString("There is no '%1' object in the model").arg(objectId);
    return {};
}

std::vector<ao_builder::Id> Model::getObjectsByInterface(QString iface)
{
    return iface == ao_builder::DBUS_LEGACY_OBJECTS_INTERFACE_NAME ? this->getObjects() : std::vector<ao_builder::Id>();
}

std::vector<ao_builder::Id> Model::getObjectsByCategory(ao_builder::Id category_id)
{
    std::vector<ao_builder::Id> objectIds{};

    for (int i = 0; i < this->objectsRoot->rowCount(); ++i)
    {
        QStandardItem *child = this->objectsRoot->child(i);
        auto item            = dynamic_cast<ModelItem *>(child);

        if (item == nullptr)
        {
            continue;
        }

        ao_builder::Object *object = item->getObject();

        if (object != nullptr && object->m_categoryId == category_id)
        {
            objectIds.push_back(object->m_id);
        }
    }
    return objectIds;
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

void Model::clear()
{
    QStandardItemModel::clear();

    this->categoriesRoot = new ModelItem();
    this->appsRoot       = new ModelItem();
    this->objectsRoot    = new ModelItem();

    QStandardItem *root = this->invisibleRootItem();
    root->appendRow(this->categoriesRoot);
    root->appendRow(this->appsRoot);
    root->appendRow(this->objectsRoot);
}

void Model::build(std::vector<std::unique_ptr<ao_builder::Object>> categories,
                  std::vector<std::unique_ptr<ao_builder::Object>> apps,
                  std::vector<std::unique_ptr<ao_builder::Object>> objects)
{
    this->clear();

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
