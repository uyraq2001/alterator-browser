#ifndef ACLOCALAPPLICATIONMODEL_H
#define ACLOCALAPPLICATIONMODEL_H

#include "aclocalapplication.h"

#include <QStandardItemModel>

class ACLocalApplicationModel : public QStandardItemModel
{
public:
    ACLocalApplicationModel();
    ~ACLocalApplicationModel();

    std::vector<ACLocalApplication *> getAppsByInterface(QString interface);

    void setLocale();
};

#endif // ACLOCALAPPLICATIONMODEL_H
