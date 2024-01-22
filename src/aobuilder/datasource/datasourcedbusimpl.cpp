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
        qWarning() << QString("The list of applications from '%1' interface is empty")
                          .arg(DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME);
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
        qWarning() << QString("The list of categries from '%1' interface is empty")
                          .arg(DBUS_CATEGORY_OBJECT_INTERFACE_NAME);
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

QStringList DataSourceDBusImpl::getObjectsPath(QString interface)
{
    return getPathsByInterface(interface);
}

QString DataSourceDBusImpl::getObjectInfo(QString path, QString interface)
{
    return getObjectInfo(interface, path, DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);
}

QStringList DataSourceDBusImpl::getPathsByInterface(QString ifaceName)
{
    QDBusInterface iface(d->m_serviceName,
                         DBUS_ALTERATOR_MANAGER_PATH,
                         DBUS_MANAGER_INTERFACE_NAME,
                         d->m_dbusConnection);

    if (!iface.isValid())
    {
        qWarning() << QString("Interface '%1' is invalid").arg(DBUS_MANAGER_INTERFACE_NAME);
        return {};
    }

    const QDBusReply<QList<QDBusObjectPath>> reply = iface.call(DBUS_GET_OBJECTS_METHOD_NAME, ifaceName);

    if (!reply.isValid())
    {
        qWarning() << QString("Reply from interface'%1', method '%2' is invalid")
                          .arg(ifaceName, DBUS_GET_OBJECTS_METHOD_NAME);
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
        qWarning() << QString("Invalid interface '%1' for object '%2'").arg(interfaceName, path);
        return {};
    }

    const QDBusReply<QStringList> reply = iface.call(listMethodName);

    if (!reply.isValid())
    {
        qWarning() << QString("Invalid reply from method '%1' of '%2' interface ").arg(interfaceName, path);
        return {};
    }

    return reply.value();
}

QByteArray DataSourceDBusImpl::getObjectInfo(QString ifaceName, QString path, QString methodName)
{
    QDBusInterface iface(d->m_serviceName, path, ifaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        qWarning() << "Invalid interface" << ifaceName << "for object" << path;
        return {};
    }

    QDBusReply<QByteArray> reply = iface.call(methodName);

    if (!reply.isValid())
    {
        qWarning() << "Invalid reply from object" << path << "with interface" << ifaceName << "and method name"
                   << methodName << ":" << reply.error().message();
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
        qWarning() << QString("Invalid interface '%1' for object '%2'").arg(ifaceName, path);
        return {};
    }

    const QDBusReply<QByteArray> reply = iface.call(methodName, param);

    if (!reply.isValid())
    {
        qWarning() << QString("Invalid reply from method '%1' with parameter '%2' of '%3' interface ")
                          .arg(ifaceName, param, path);
        return {};
    }

    return reply.value();
}

} // namespace ao_builder
