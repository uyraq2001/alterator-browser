#ifndef ACLOCALAPPLICATIONITEM_H
#define ACLOCALAPPLICATIONITEM_H

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

#endif // ACLOCALAPPLICATIONITEM_H
