#include "objectitem.h"

ACObjectItem::ACObjectItem()
    : QStandardItem()
    , m_itemType(ITEM_TYPE::category)
    , m_acObject(new ACObject())
{}

ACObjectItem::~ACObjectItem() {}

int ACObjectItem::type() const
{
    return static_cast<int>(m_itemType);
}

ACObject *ACObjectItem::getACObject()
{
    return m_acObject.get();
}
