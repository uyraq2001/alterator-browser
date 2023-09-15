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
    ~ObjectItem() = default;

    int type() const override;

    AbstractEntity *getObject();

public:
    ItemType m_itemType;

    std::unique_ptr<AbstractEntity> m_object{nullptr};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_ITEM_H
