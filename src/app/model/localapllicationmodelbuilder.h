#ifndef ACLOCALAPLLICATIONMODELBUILDER_H
#define ACLOCALAPLLICATIONMODELBUILDER_H

#include <memory>
#include <QDBusConnection>
#include <QString>

#include "localapplication.h"
#include "localapplicationmodel.h"

class ACLocalApllicationModelBuilder
{
public:
    ACLocalApllicationModelBuilder(QString serviceName,
                                   QString dbusPath,
                                   QString interfaceName,
                                   QString getListOfFilesMethod,
                                   QString getDesktopFileMethod);

    std::unique_ptr<ACLocalApplicationModel> buildModel();

private:
    QStringList getListOfDesktopFiles();

    std::vector<std::unique_ptr<ACLocalApplication>> parseDesktopFiles(QStringList files);

    QString getDesktopFile(QString file);

private:
    QDBusConnection m_dbusConnection;
    QString m_dbusServiceName;
    QString m_dbusPath;
    QString m_interface;
    QString m_getFilesMethodName;
    QString m_getDesktopFileMethodName;
};

#endif // ACLOCALAPLLICATIONMODELBUILDER_H
