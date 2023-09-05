#include "objectitem.h"

namespace ab
{
namespace model
{
ObjectItem::ObjectItem()
    : m_object(new Object())
{}

int ObjectItem::type() const
{
    return static_cast<int>(m_itemType);
}

Object *ObjectItem::getObject()
{
    return m_object.get();
}
} // namespace model
} // namespace ab
