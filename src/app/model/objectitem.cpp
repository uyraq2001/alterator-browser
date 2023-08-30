#include "objectitem.h"

ObjectItem::ObjectItem()
    : QStandardItem()
    , m_itemType(ITEM_TYPE::category)
    , m_acObject(new Object())
{}

ObjectItem::~ObjectItem() {}

int ObjectItem::type() const
{
    return static_cast<int>(m_itemType);
}

Object *ObjectItem::getACObject()
{
    return m_acObject.get();
}
