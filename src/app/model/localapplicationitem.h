#ifndef AB_LOCAL_APPLICATION_ITEM_H
#define AB_LOCAL_APPLICATION_ITEM_H

#include "localapplication.h"

#include <QStandardItemModel>

class LocalApplicationItem : public QStandardItem
{
public:
    LocalApplicationItem();

    LocalApplication *getACLocalApplicationObject();

    void setLocale(QString locale);

public:
    std::unique_ptr<LocalApplication> m_acLocalApplication;
};

#endif // AB_LOCAL_APPLICATION_ITEM_H
