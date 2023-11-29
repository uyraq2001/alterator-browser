#include "datasourcedbusimpl.h"
#include "constants.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include <iostream>
#include <utility>
#include <QDebug>

namespace ao_builder
{
class DataSourseDbusImplPrivate
{
public:
    DataSourseDbusImplPrivate(QString serviceName)
        : m_serviceName(std::move(serviceName))
        , m_dbusConnection(QDBusConnection::systemBus())
    {}

    QString m_serviceName;
    QDBusConnection m_dbusConnection;
};

DataSourceDBusImpl::DataSourceDBusImpl(QString serviceName)
    : d(new DataSourseDbusImplPrivate(serviceName))
{}

DataSourceDBusImpl::~DataSourceDBusImpl()
{
    delete d;
}

QStringList DataSourceDBusImpl::getLocalAppPaths()
{
    const QStringList mainAppObjectList = getPathsByInterface(DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME);

    if (mainAppObjectList.empty())
    {
        return {};
    }

    const QString &mainObjectPath = mainAppObjectList.first();

    return getObjectsList(DBUS_OBJECT_METHOD_LIST_DEFAULT_NAME, mainObjectPath, DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME);
}

QString DataSourceDBusImpl::getLocalAppInfo(QString path)
{
    return getObjectInfoByName(DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME,
                               DBUS_LOCAL_APP_OBJECT_PATH,
                               path,
                               DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);
}

QStringList DataSourceDBusImpl::getCategoriesList()
{
    const QStringList mainCatObjectList = getPathsByInterface(DBUS_CATEGORY_OBJECT_INTERFACE_NAME);

    if (mainCatObjectList.empty())
    {
        return {};
    }

    const QString &mainCatObjectPath = mainCatObjectList.first();

    return getObjectsList(DBUS_OBJECT_METHOD_LIST_DEFAULT_NAME, mainCatObjectPath, DBUS_CATEGORY_OBJECT_INTERFACE_NAME);
}

QString DataSourceDBusImpl::getCategoryInfo(QString path)
{
    return getObjectInfoByName(DBUS_CATEGORY_OBJECT_INTERFACE_NAME,
                               DBUS_CATEGORY_OBJECT_PATH,
                               path,
                               DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);
}

QStringList DataSourceDBusImpl::getLegacyObjectsPaths()
{
    return getPathsByInterface(DBUS_LEGACY_OBJECT_INTERFACE_NAME);
}

QString DataSourceDBusImpl::getLegacyObjectInfo(QString path)
{
    return getObjectInfo(DBUS_LEGACY_OBJECT_INTERFACE_NAME, path, DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);
}

QStringList DataSourceDBusImpl::getObjectsPath()
{
    return getPathsByInterface(DBUS_OBJECT_INTERFACE_NAME);
}

QString DataSourceDBusImpl::getObjectInfo([[maybe_unused]] QString path)
{
    return {};
}

QStringList DataSourceDBusImpl::getPathsByInterface(QString ifaceName)
{
    QDBusInterface iface(d->m_serviceName,
                         DBUS_ALTERATOR_MANAGER_PATH,
                         DBUS_MANAGER_INTERFACE_NAME,
                         d->m_dbusConnection);

    if (!iface.isValid())
    {
        return {};
    }

    const QDBusReply<QList<QDBusObjectPath>> reply = iface.call(DBUS_GET_OBJECTS_METHOD_NAME, ifaceName);

    if (!reply.isValid())
    {
        return {};
    }

    QStringList result{};

    for (const QDBusObjectPath &path : reply.value())
    {
        result.append(path.path());
    }

    return result;
}

QStringList DataSourceDBusImpl::getObjectsList(QString listMethodName, QString path, QString interfaceName)
{
    QDBusInterface iface(d->m_serviceName, path, interfaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        return {};
    }

    const QDBusReply<QStringList> reply = iface.call(listMethodName);

    if (!reply.isValid())
    {
        return {};
    }

    return reply.value();
}

QByteArray DataSourceDBusImpl::getObjectInfo(QString ifaceName, QString path, QString methodName)
{
    QDBusInterface iface(d->m_serviceName, path, ifaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        return {};
    }

    QDBusReply<QByteArray> reply = iface.call(methodName);

    if (!reply.isValid())
    {
        return {};
    }

    return reply.value();
}

QByteArray DataSourceDBusImpl::getObjectInfoByName(QString ifaceName,
                                                   QString path,
                                                   QString objectName,
                                                   QString methodName)
{
    const QString param = QString("%1%2%3").arg("\"").arg(objectName).arg("\"");

    QDBusInterface iface(d->m_serviceName, path, ifaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        return {};
    }

    const QDBusReply<QByteArray> reply = iface.call(methodName, param);

    if (!reply.isValid())
    {
        return {};
    }

    return reply.value();
}

} // namespace ao_builder
