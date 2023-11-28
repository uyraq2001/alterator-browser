#include "datasourcedbusimpl.h"
#include "constants.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#include <iostream>
#include <QDebug>

namespace ao_builder
{
class DataSourseDbusImplPrivate
{
public:
    DataSourseDbusImplPrivate(QString serviceName)
        : m_serviceName(serviceName)
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
    QStringList mainAppObjectList = getPathsByInterface(DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME);

    if (mainAppObjectList.empty())
    {
        return {};
    }

    QString mainObjectPath = mainAppObjectList.at(0);

    QStringList localAppList = getObjectsList(DBUS_OBJECT_METHOD_LIST_DEFAULT_NAME,
                                              mainObjectPath,
                                              DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME);

    return localAppList;
}

QString DataSourceDBusImpl::getLocalAppInfo(QString path)
{
    QString info = getObjectInfoByName(DBUS_LOCAL_APP_OBJECT_INTERFACE_NAME,
                                       DBUS_LOCAL_APP_OBJECT_PATH,
                                       path,
                                       DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);

    return info;
}

QStringList DataSourceDBusImpl::getCategoriesList()
{
    QStringList mainCatObjectList = getPathsByInterface(DBUS_CATEGORY_OBJECT_INTERFACE_NAME);

    if (mainCatObjectList.empty())
    {
        return {};
    }

    QString mainCatObjectPath = mainCatObjectList.at(0);

    QStringList localCatList = getObjectsList(DBUS_OBJECT_METHOD_LIST_DEFAULT_NAME,
                                              mainCatObjectPath,
                                              DBUS_CATEGORY_OBJECT_INTERFACE_NAME);

    return localCatList;
}

QString DataSourceDBusImpl::getCategoryInfo(QString path)
{
    QString catInfo = getObjectInfoByName(DBUS_CATEGORY_OBJECT_INTERFACE_NAME,
                                          DBUS_CATEGORY_OBJECT_PATH,
                                          path,
                                          DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);

    return catInfo;
}

QStringList DataSourceDBusImpl::getLegacyObjectsPaths()
{
    QStringList legacyObjectList = getPathsByInterface(DBUS_LEGACY_OBJECT_INTERFACE_NAME);

    return legacyObjectList;
}

QString DataSourceDBusImpl::getLegacyObjectInfo(QString path)
{
    QString legacyInfo = getObjectInfo(DBUS_LEGACY_OBJECT_INTERFACE_NAME, path, DBUS_OBJECT_METHOD_INFO_DEFAULT_NAME);

    return legacyInfo;
}

QStringList DataSourceDBusImpl::getObjectsPath()
{
    QStringList objectList = getPathsByInterface(DBUS_OBJECT_INTERFACE_NAME);

    return objectList;
}

QString DataSourceDBusImpl::getObjectInfo(QString path)
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
        return QStringList{};
    }

    QDBusReply<QList<QDBusObjectPath>> reply = iface.call(DBUS_GET_OBJECTS_METHOD_NAME, ifaceName);

    if (!reply.isValid())
    {
        return QStringList{};
    }

    QStringList result{};

    for (QDBusObjectPath &path : reply.value())
    {
        result.append(path.path());
    }

    if (result.isEmpty())
    {
        return {};
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

    QDBusReply<QStringList> reply = iface.call(listMethodName);

    if (!reply.isValid())
    {
        return QStringList{};
    }

    return reply.value();
}

QByteArray DataSourceDBusImpl::getObjectInfo(QString ifaceName, QString path, QString methodName)
{
    QDBusInterface iface(d->m_serviceName, path, ifaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        return QByteArray{};
    }

    QDBusReply<QByteArray> reply = iface.call(methodName);

    if (!reply.isValid())
    {
        return QByteArray{};
    }

    return reply.value();
}

QByteArray DataSourceDBusImpl::getObjectInfoByName(QString ifaceName,
                                                   QString path,
                                                   QString objectName,
                                                   QString methodName)
{
    QString param = QString("%1%2%3").arg("\"").arg(objectName).arg("\"");

    QDBusInterface iface(d->m_serviceName, path, ifaceName, d->m_dbusConnection);

    if (!iface.isValid())
    {
        return QByteArray{};
    }

    QDBusReply<QByteArray> reply = iface.call(methodName, param);

    if (!reply.isValid())
    {
        return QByteArray{};
    }

    return reply.value();
}

} // namespace ao_builder
