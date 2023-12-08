#ifndef AOB_DATA_SOURCE_INTERFACE_H
#define AOB_DATA_SOURCE_INTERFACE_H

#include <QByteArray>
#include <QObject>
#include <QString>

namespace ao_builder
{
class DataSourceInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~DataSourceInterface() = default;

    virtual QStringList getLocalAppPaths()        = 0;
    virtual QString getLocalAppInfo(QString path) = 0;

    virtual QStringList getCategoriesList()       = 0;
    virtual QString getCategoryInfo(QString path) = 0;

    virtual QStringList getObjectsPath(QString interface)          = 0;
    virtual QString getObjectInfo(QString path, QString interface) = 0;

signals:
    void dataUpdated();
};

} // namespace ao_builder
#endif // AOB_DATA_SOURCE_INTERFACE_H
