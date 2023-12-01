#include "model.h"

#include "../../aobuilder/builders/aobuilderimpl.h"
#include "objectitem.h"

#include <QDebug>
#include <QStandardItemModel>
#include <qstandarditemmodel.h>

#include <memory>
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
    auto legacyObjects      = modelBuilder->buildObjects();
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
    std::vector<ao_builder::Id> categoriesIds;

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
