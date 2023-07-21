#ifndef ACLOCALAPPLICATIONITEM_H
#define ACLOCALAPPLICATIONITEM_H

#include "aclocalapplication.h"

#include <QStandardItemModel>

class ACLocalApplicationItem : public QStandardItem
{
public:
    ACLocalApplicationItem();

    ACLocalApplication *getACLocalApplicationObject();

    void setLocale(QString locale);

public:
    std::unique_ptr<ACLocalApplication> m_acLocalApplication;
};

#endif // ACLOCALAPPLICATIONITEM_H
