#include "localapllicationmodelbuilder.h"
#include "desktopfileparser.h"
#include "localapplicationbuilder.h"
#include "localapplicationitem.h"

#include <memory>
#include <utility>

#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

namespace ab
{
namespace model
{
LocalApllicationModelBuilder::LocalApllicationModelBuilder(QString serviceName,
                                                           QString dbusPath,
                                                           QString interfaceName,
                                                           QString getListOfFilesMethod,
                                                           QString getDesktopFileMethod)
    : m_dbusConnection(QDBusConnection::systemBus())
    , m_dbusServiceName(std::move(serviceName))
    , m_dbusPath(std::move(dbusPath))
    , m_interface(std::move(interfaceName))
    , m_getFilesMethodName(std::move(getListOfFilesMethod))
    , m_getDesktopFileMethodName(std::move(getDesktopFileMethod))
{}

std::unique_ptr<LocalApplicationModel> LocalApllicationModelBuilder::buildModel()
{
    QStringList listOfDesktopFiles = getListOfDesktopFiles();

    if (listOfDesktopFiles.isEmpty())
    {
        qCritical() << "Can't get list of local applications!";
        return std::make_unique<LocalApplicationModel>();
    }

    std::vector<std::unique_ptr<LocalApplication>> listOfApps = parseDesktopFiles(listOfDesktopFiles);

    if (listOfApps.empty())
    {
        qCritical() << "Can't create LocalApplications objects!";
        return std::make_unique<LocalApplicationModel>();
    }

    auto model = std::make_unique<LocalApplicationModel>();
    auto rootItem = model.get()->invisibleRootItem();

    for (auto &listOfApp : listOfApps)
    {
        auto newItem                  = new LocalApplicationItem();
        newItem->m_acLocalApplication = std::move(listOfApp);

        rootItem->appendRow(newItem);
    }

    return model;
}

QStringList LocalApllicationModelBuilder::getListOfDesktopFiles()
{
    QDBusInterface iface(m_dbusServiceName, m_dbusPath, m_interface, m_dbusConnection);

    if (!iface.isValid())
    {
        qCritical() << "Can't access alterator manager interface to build local applications model!";
        return {};
    }

    QDBusReply<QStringList> reply = iface.call(m_getFilesMethodName);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface to build local applications model!";
        return {};
    }

    QStringList listOfDesktopFiles = reply.value();

    return listOfDesktopFiles;
}

std::vector<std::unique_ptr<LocalApplication>> LocalApllicationModelBuilder::parseDesktopFiles(QStringList files)
{
    std::vector<std::unique_ptr<LocalApplication>> result;

    for (QString currentFile : files)
    {
        QString currentFileData = getDesktopFile(currentFile);

        if (currentFile.isEmpty())
        {
            continue;
        }

        DesktopFileParser parser(currentFileData);
        LocalApplicationBuilder builder;
        std::unique_ptr<LocalApplication> newLocalApp = builder.buildLocalApplicationObject(parser);

        if (newLocalApp)
        {
            result.push_back(std::move(newLocalApp));
        }
    }

    return result;
}

QString LocalApllicationModelBuilder::getDesktopFile(QString file)
{
    QString result;

    QDBusInterface iface(m_dbusServiceName, m_dbusPath, m_interface, m_dbusConnection);

    if (!iface.isValid())
    {
        qCritical() << "Can't access alterator manager interface to get desktop file: " << file;
        return result;
    }

    QDBusReply<QByteArray> reply = iface.call(m_getDesktopFileMethodName, file);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface to to get desktop file: " << file;
        return result;
    }

    result = QString(reply.value());

    return result;
}
} // namespace model
} // namespace ab
