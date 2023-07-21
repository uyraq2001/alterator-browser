#ifndef ACLOCALAPPLICATIONMODEL_H
#define ACLOCALAPPLICATIONMODEL_H

#include <QStandardItemModel>

class ACLocalApplicationModel : public QStandardItemModel
{
public:
    ACLocalApplicationModel();
    ~ACLocalApplicationModel();

    void setLocale(QString locale);
};

#endif // ACLOCALAPPLICATIONMODEL_H
