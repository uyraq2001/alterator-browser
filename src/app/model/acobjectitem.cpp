#include "acobjectitem.h"
#include "fulldefinitions.h"

ACObjectItem::ACObjectItem()
    : QStandardItem()
    , m_itemType(ITEM_TYPE::category)
    , m_acEntity(new ACObject())
{}

ACObjectItem::~ACObjectItem() {}

int ACObjectItem::type() const
{
    return static_cast<int>(m_itemType);
}

AbstractACEntity *ACObjectItem::getACEntity()
{
    return m_acEntity.get();
}

void ACObjectItem::setEntity(std::unique_ptr<AbstractACEntity> entity)
{
    m_acEntity = std::move(entity);
}
