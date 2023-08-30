#include "localapllicationmodelbuilder.h"
#include "localapplicationbuilder.h"
#include "localapplicationitem.h"
#include "desktopfileparser.h"

#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

ACLocalApllicationModelBuilder::ACLocalApllicationModelBuilder(QString serviceName,
                                                               QString dbusPath,
                                                               QString interfaceName,
                                                               QString getListOfFilesMethod,
                                                               QString getDesktopFileMethod)
    : m_dbusConnection(QDBusConnection::systemBus())
    , m_dbusServiceName(serviceName)
    , m_dbusPath(dbusPath)
    , m_interface(interfaceName)
    , m_getFilesMethodName(getListOfFilesMethod)
    , m_getDesktopFileMethodName(getDesktopFileMethod)
{}

std::unique_ptr<ACLocalApplicationModel> ACLocalApllicationModelBuilder::buildModel()
{
    QStringList listOfDesktopFiles = getListOfDesktopFiles();

    if (listOfDesktopFiles.isEmpty())
    {
        qCritical() << "Can't get list of local applications!";

        return std::unique_ptr<ACLocalApplicationModel>(new ACLocalApplicationModel());
    }

    std::vector<std::unique_ptr<ACLocalApplication>> listOfApps = parseDesktopFiles(listOfDesktopFiles);

    if (listOfApps.empty())
    {
        qCritical() << "Can't create ACLocalApplications objects!";

        return std::unique_ptr<ACLocalApplicationModel>(new ACLocalApplicationModel());
    }

    std::unique_ptr<ACLocalApplicationModel> model(new ACLocalApplicationModel);

    auto rootItem = model.get()->invisibleRootItem();

    for (size_t i = 0; i < listOfApps.size(); ++i)
    {
        ACLocalApplicationItem *newItem = new ACLocalApplicationItem();
        newItem->m_acLocalApplication   = std::move(listOfApps.at(i));

        rootItem->appendRow(newItem);
    }

    return model;
}

QStringList ACLocalApllicationModelBuilder::getListOfDesktopFiles()
{
    QDBusInterface iface(m_dbusServiceName, m_dbusPath, m_interface, m_dbusConnection);

    if (!iface.isValid())
    {
        qCritical() << "Can't access alterator manager interface to build local applications model!";

        return QStringList();
    }

    QDBusReply<QStringList> reply = iface.call(m_getFilesMethodName);

    if (!reply.isValid())
    {
        qCritical() << "Can't get reply from alterator manager interface to build local applications model!";

        return QStringList();
    }

    QStringList listOfDesktopFiles = reply.value();

    return listOfDesktopFiles;
}

std::vector<std::unique_ptr<ACLocalApplication>> ACLocalApllicationModelBuilder::parseDesktopFiles(QStringList files)
{
    std::vector<std::unique_ptr<ACLocalApplication>> result;

    for (QString currentFile : files)
    {
        QString currentFileData = getDesktopFile(currentFile);

        if (currentFile.isEmpty())
        {
            continue;
        }

        DesktopFileParser parser(currentFileData);

        ACLocalApplicationBuilder builder;

        std::unique_ptr<ACLocalApplication> newACLocalApp = builder.buildACLocalApplicationObject(parser);

        if (newACLocalApp)
        {
            result.push_back(std::move(newACLocalApp));
        }
    }

    return result;
}

QString ACLocalApllicationModelBuilder::getDesktopFile(QString file)
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
