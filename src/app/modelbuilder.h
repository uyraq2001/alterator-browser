#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <QObject>
#include <QStandardItemModel>
#include <QDBusMessage>

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

    QVariantMap findTranslations(QString field, QMap<QString, QString> dump);
    QMap<QString, QString> readIniFromDBus(QDBusMessage mess);
};

#endif // MODELBUILDER_H
