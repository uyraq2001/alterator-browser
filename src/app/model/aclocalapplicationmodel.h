#ifndef ACLOCALAPPLICATIONMODEL_H
#define ACLOCALAPPLICATIONMODEL_H

#include <QStandardItemModel>

class ACLocalApplicationModel : public QStandardItemModel
{
public:
    ACLocalApplicationModel();
    ~ACLocalApplicationModel();

    void setLocale();
};

#endif // ACLOCALAPPLICATIONMODEL_H
