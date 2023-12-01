#ifndef AB_MODEL_OBJECT_ITEM_H
#define AB_MODEL_OBJECT_ITEM_H

#include <QStandardItem>

#include "../../aobuilder/objects/object.h"

namespace ab::model
{
class ModelItem : public QStandardItem
{
public:
    enum class ItemType
    {
        None = QStandardItem::UserType + 1,
        Category,
        LocalApplication,
        Object,
        LegacyObject
    };

public:
    ModelItem();
    ModelItem(ItemType, std::unique_ptr<ao_builder::Object>);

public:
    ModelItem(ModelItem &)                  = delete;
    ModelItem(ModelItem &&)                 = delete;
    ModelItem &operator=(const ModelItem &) = delete;
    ModelItem &operator=(ModelItem &&)      = delete;
    ~ModelItem() override                   = default;

    [[nodiscard]] int type() const override;

    ao_builder::Object *getObject();

public:
    ItemType m_itemType;

    std::unique_ptr<ao_builder::Object> m_object{nullptr};
};
} // namespace ab::model

#endif // AB_MODEL_OBJECT_ITEM_H
