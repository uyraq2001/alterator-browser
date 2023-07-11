#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <QObject>
#include <QStandardItemModel>

class ModelBuilder : public QObject
{
    Q_OBJECT
public:
    ModelBuilder(QObject *parent = nullptr);

    bool build(QStandardItemModel *model);

private:
    ModelBuilder(const ModelBuilder &) = delete;
    ModelBuilder(ModelBuilder &&)      = delete;
    ModelBuilder &operator=(const ModelBuilder &) = delete;
    ModelBuilder &operator=(ModelBuilder &&) = delete;

    QVariantMap findTraslations(QString field, QMap<QString, QString> dump);
};

#endif // MODELBUILDER_H
