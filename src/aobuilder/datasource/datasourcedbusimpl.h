#ifndef AOB_DATA_SOURCE_DBUS_IMPL_H
#define AOB_DATA_SOURCE_DBUS_IMPL_H

#include "datasourceinterface.h"

#include <QString>

namespace ao_builder
{
class DataSourseDbusImplPrivate;

class DataSourceDBusImpl : public DataSourceInterface
{
public:
    DataSourceDBusImpl(QString serviceName);
    ~DataSourceDBusImpl() override;

public:
    QStringList getLocalAppPaths() override;
    QString getLocalAppInfo(QString path) override;

    QStringList getCategoriesList() override;
    QString getCategoryInfo(QString path) override;

    QStringList getObjectsPath(QString interface) override;
    QString getObjectInfo(QString path, QString interface) override;

public:
    DataSourceDBusImpl(const DataSourceDBusImpl &) = delete;
    DataSourceDBusImpl(DataSourceDBusImpl &&)      = delete;
    DataSourceDBusImpl &operator=(const DataSourceDBusImpl &) = delete;
    DataSourceDBusImpl &operator=(DataSourceDBusImpl &&) = delete;

private:
    QByteArray getObjectInfo(QString ifaceName, QString path, QString methodName);
    QByteArray getObjectInfoByName(QString ifaceName, QString path, QString objectName, QString methodName);

    QStringList getPathsByInterface(QString ifaceName);
    QStringList getObjectsList(QString listMethodName, QString path, QString interfaceName);

private slots:
    void dbusUpdated(QString, QString, QString);

private:
    DataSourseDbusImplPrivate *d;
};
} // namespace ao_builder

#endif // AOB_DATA_SOURCE_DBUS_IMPL_H
