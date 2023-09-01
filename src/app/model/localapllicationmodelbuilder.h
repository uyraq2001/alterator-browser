#ifndef AB_LOCAL_APLLICATION_MODEL_BUILDER_H
#define AB_LOCAL_APLLICATION_MODEL_BUILDER_H

#include "localapplication.h"
#include "localapplicationmodel.h"

#include <memory>

#include <QDBusConnection>
#include <QString>

namespace ab
{
namespace model
{
class LocalApllicationModelBuilder
{
public:
    LocalApllicationModelBuilder(QString serviceName,
                                 QString dbusPath,
                                 QString interfaceName,
                                 QString getListOfFilesMethod,
                                 QString getDesktopFileMethod);

    std::unique_ptr<LocalApplicationModel> buildModel();

private:
    QStringList getListOfDesktopFiles();
    std::vector<std::unique_ptr<LocalApplication>> parseDesktopFiles(QStringList files);
    QString getDesktopFile(QString file);

private:
    QDBusConnection m_dbusConnection;
    QString m_dbusServiceName;
    QString m_dbusPath;
    QString m_interface;
    QString m_getFilesMethodName;
    QString m_getDesktopFileMethodName;
};
} // namespace model
} // namespace ab

#endif // AB_LOCAL_APLLICATION_MODEL_BUILDER_H
