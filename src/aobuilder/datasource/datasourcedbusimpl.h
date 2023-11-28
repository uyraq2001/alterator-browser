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
    ~DataSourceDBusImpl();

public:
    QStringList getLocalAppPaths() override;
    virtual QString getLocalAppInfo(QString path) override;

    QStringList getCategoriesList() override;
    virtual QString getCategoryInfo(QString path) override;

    QStringList getLegacyObjectsPaths() override;
    virtual QString getLegacyObjectInfo(QString path) override;

    QStringList getObjectsPath() override;
    virtual QString getObjectInfo(QString path) override;

private:
    QByteArray getObjectInfo(QString ifaceName, QString path, QString methodName);
    QByteArray getObjectInfoByName(QString ifaceName, QString path, QString objectName, QString methodName);

    QStringList getPathsByInterface(QString ifaceName);
    QStringList getObjectsList(QString listMethodName, QString path, QString interfaceName);

private:
    DataSourseDbusImplPrivate *d;

private:
    DataSourceDBusImpl(const DataSourceDBusImpl &) = delete;
    DataSourceDBusImpl(DataSourceDBusImpl &&)      = delete;
    DataSourceDBusImpl &operator=(const DataSourceDBusImpl &) = delete;
    DataSourceDBusImpl &operator=(DataSourceDBusImpl &&) = delete;
};

} // namespace ao_builder

#endif // AOB_DATA_SOURCE_DBUS_IMPL_H
