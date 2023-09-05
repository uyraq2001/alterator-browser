#ifndef AB_OBJECT_ITEM_H
#define AB_OBJECT_ITEM_H

#include "object.h"

#include <QStandardItem>

namespace ab
{
namespace model
{
class ObjectItem : public QStandardItem
{
public:
    enum class ItemType
    {
        category = QStandardItem::UserType + 1,
        module
    };

public:
    ObjectItem();
    ~ObjectItem() override = default;

public:
    ObjectItem(const ObjectItem &) = delete;
    ObjectItem(ObjectItem &&)      = delete;
    ObjectItem &operator=(const ObjectItem &) = delete;
    ObjectItem &operator=(ObjectItem &&) = delete;

    int type() const override;
    Object *getObject();

public:
    ItemType m_itemType = ItemType::category;
    std::unique_ptr<Object> m_object{nullptr};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_ITEM_H
