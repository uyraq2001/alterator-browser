#include "objectitem.h"

namespace ab
{
namespace model
{
ObjectItem::ObjectItem()
    : m_itemType(ItemType::category)
{}

int ObjectItem::type() const
{
    return static_cast<int>(m_itemType);
}

std::variant<Object, Category> *ObjectItem::getObject()
{
    return m_object.get();
}
} // namespace model
} // namespace ab
