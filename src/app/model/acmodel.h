#ifndef ACMODEL_H
#define ACMODEL_H

#include <QStandardItemModel>

class ACModel : public QStandardItemModel
{
public:
    ACModel();
    ~ACModel() = default;

    void translateModel(QString locale);

private:
    void translateItem(QStandardItem *item, QString locale);

private:
    ACModel(const ACModel &) = delete;
    ACModel(ACModel &&)      = delete;
    ACModel &operator=(const ACModel &) = delete;
    ACModel &operator=(ACModel &&) = delete;
};

#endif // ACMODEL_H
