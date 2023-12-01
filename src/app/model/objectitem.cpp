#include "objectitem.h"
#include <memory>

namespace ab::model
{
ModelItem::ModelItem()
    : m_itemType(ItemType::None)
{}

ModelItem::ModelItem(ItemType type, std::unique_ptr<ao_builder::Object> object)
    : m_itemType(type)
    , m_object(std::move(object))
{}

int ModelItem::type() const
{
    return static_cast<int>(m_itemType);
}

ao_builder::Object *ModelItem::getObject()
{
    return m_object.get();
}

} // namespace ab::model
