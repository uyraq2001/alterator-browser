#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <QObject>
#include <QStandardItemModel>

class ModelBuilder : public QObject
{
    Q_OBJECT
public:
    ModelBuilder(QStandardItemModel *m, QObject *parent = nullptr);

    bool build();

private:
    QStandardItemModel *model;

    ModelBuilder(const ModelBuilder &) = delete;
    ModelBuilder(ModelBuilder &&)      = delete;
    ModelBuilder &operator=(const ModelBuilder &) = delete;
    ModelBuilder &operator=(ModelBuilder &&) = delete;
};

#endif // MODELBUILDER_H
