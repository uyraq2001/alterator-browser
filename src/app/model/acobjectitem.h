#ifndef ACOBJECTITEM_H
#define ACOBJECTITEM_H

#include "abstractacentity.h"

#include <QStandardItem>

class ACObjectItem : public QStandardItem
{
public:
    enum class ITEM_TYPE
    {
        category = QStandardItem::UserType + 1,
        module
    };

public:
    ACObjectItem();

    ~ACObjectItem();

    int type() const override;

    AbstractACEntity *getACEntity();
    void setEntity(std::unique_ptr<AbstractACEntity> entity);

public:
    ITEM_TYPE m_itemType;

    std::unique_ptr<AbstractACEntity> m_acEntity;
};

#endif // ACOBJECTITEM_H
