/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "adtmodelbuilderstrategydbusinfodesktop.h"
#include "../core/adtdesktopfileparser.h"

#include <QDBusReply>
#include <QDebug>
#include <QJsonDocument>

const QString ADTModelBuilderStrategyDbusInfoDesktop::LIST_METHOD = QString("list");
const QString ADTModelBuilderStrategyDbusInfoDesktop::INFO_METHOD = QString("info");

ADTModelBuilderStrategyDbusInfoDesktop::ADTModelBuilderStrategyDbusInfoDesktop(QString serviceName,
                                                                               QString path,
                                                                               QString interface,
                                                                               QString getMethodName,
                                                                               QString findInterface,
                                                                               QString runTaskMethodName,
                                                                               TreeModelBuilderInterface *builder)
    : m_serviceName(serviceName)
    , m_path(path)
    , m_interface(interface)
    , m_get_method_name(getMethodName)
    , m_findInterface(findInterface)
    , m_runTaskMethodName(runTaskMethodName)
    , m_treeModelBuilder(builder)
    , m_implementedInterfacesPath()
    , m_dbus(new QDBusConnection(QDBusConnection::systemBus()))
    , m_dbusInterface(new QDBusInterface(m_serviceName, m_path, m_interface, *m_dbus.get()))
{}

std::unique_ptr<TreeModel> ADTModelBuilderStrategyDbusInfoDesktop::buildModel()
{
    QStringList listOfObjects = getObjectsPathByInterface(m_findInterface);

    if (listOfObjects.isEmpty())
    {
        return nullptr;
    }

    std::vector<std::unique_ptr<ADTExecutable>> adtExecutables;

    for (QString currentPath : listOfObjects)
    {
        std::vector<std::unique_ptr<ADTExecutable>> currentExecutables = buildADTExecutablesFromDesktopFile(currentPath);
        if (!currentExecutables.empty())
        {
            for (auto &currentExe : currentExecutables)
            {
                adtExecutables.push_back(std::move(currentExe));
            }
        }
    }

    return m_treeModelBuilder->buildModel(std::move(adtExecutables));
}

QStringList ADTModelBuilderStrategyDbusInfoDesktop::getObjectsPathByInterface(QString interface)
{
    QDBusReply<QList<QDBusObjectPath>> reply = m_dbusInterface->call(m_get_method_name, interface);

    QList<QDBusObjectPath> pathList = reply.value();

    QStringList paths;

    std::for_each(pathList.begin(), pathList.end(), [&paths](QDBusObjectPath &path) { paths.append(path.path()); });

    return paths;
}

std::vector<std::unique_ptr<ADTExecutable>> ADTModelBuilderStrategyDbusInfoDesktop::buildADTExecutablesFromDesktopFile(
    QString path)
{
    QDBusInterface iface(m_serviceName, path, m_findInterface, *m_dbus.get());

    QDBusReply<QByteArray> testsListReply = iface.call(ADTModelBuilderStrategyDbusInfoDesktop::LIST_METHOD);

    QByteArray testsList = testsListReply.value();

    QString listResultMethod = QString::fromStdString(testsList.toStdString());

    if (listResultMethod.isEmpty())
    {
        qWarning() << "ERROR! Can't get list of tests from object with path: " << path;

        return std::vector<std::unique_ptr<ADTExecutable>>();
    }

    QStringList listOfTests = listResultMethod.split("\n");

    QDBusReply<QByteArray> reply = iface.call(ADTModelBuilderStrategyDbusInfoDesktop::INFO_METHOD);

    QByteArray infoList = reply.value();

    QString listInfo = QString::fromStdString(infoList.toStdString());

    if (listInfo.isEmpty())
    {
        qWarning() << "ERROR! Can't get info from object with path: " << path;

        return std::vector<std::unique_ptr<ADTExecutable>>();
    }

    ADTDesktopFileParser parser(listInfo, listOfTests, m_serviceName, path, m_findInterface, m_runTaskMethodName);

    return parser.buildExecutables();
}
