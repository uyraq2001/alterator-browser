#ifndef ACOBJECTITEM_H
#define ACOBJECTITEM_H

#include "acobject.h"

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

    ACObject *getACObject();

public:
    ITEM_TYPE m_itemType;

    std::unique_ptr<ACObject> m_acObject;
};

#endif // ACOBJECTITEM_H
