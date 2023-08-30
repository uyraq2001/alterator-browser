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
    enum class ITEM_TYPE
    {
        category = QStandardItem::UserType + 1,
        module
    };

public:
    ObjectItem();

    ~ObjectItem();

    int type() const override;

    Object *getACObject();

public:
    ITEM_TYPE m_itemType;

    std::unique_ptr<Object> m_acObject;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_ITEM_H
