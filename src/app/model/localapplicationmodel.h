#ifndef ACLOCALAPPLICATIONMODEL_H
#define ACLOCALAPPLICATIONMODEL_H

#include "localapplication.h"

#include <QStandardItemModel>

class LocalApplicationModel : public QStandardItemModel
{
public:
    LocalApplicationModel();
    ~LocalApplicationModel();

    std::vector<LocalApplication *> getAppsByInterface(QString interface);

    void setLocale();
};

#endif // ACLOCALAPPLICATIONMODEL_H
