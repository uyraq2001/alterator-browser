#ifndef AB_OBJECT_ITEM_H
#define AB_OBJECT_ITEM_H

#include "category.h"
#include "object.h"

#include <variant>

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
    ObjectItem(ObjectItem &)  = delete;
    ObjectItem(ObjectItem &&) = delete;
    ObjectItem &operator=(const ObjectItem &) = delete;
    ObjectItem &operator=(ObjectItem &&) = delete;
    ~ObjectItem()                        = default;

    int type() const override;

    std::variant<Object, Category> *getObject();

public:
    ItemType m_itemType;

    std::unique_ptr<std::variant<Object, Category>> m_object{nullptr};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_ITEM_H
