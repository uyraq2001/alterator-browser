#ifndef ACOBJECTSMODELBUILDER_H
#define ACOBJECTSMODELBUILDER_H

#include "acmodel.h"
#include "acobject.h"
#include "acobjectitem.h"

#include <QDBusConnection>
#include <QStandardItemModel>

class ACObjectsModelBuilder
{
public:
    ACObjectsModelBuilder(QString serviceName,
                          QString dbusPath,
                          QString managerIface,
                          QString findInterface,
                          QString getObjectMethodName,
                          QString infoMethodName,
                          QString categoryMethodName);

    std::unique_ptr<ACModel> buildModel();

private:
    QStringList getListOfACObjects();

    std::vector<std::unique_ptr<ACObject>> parseACObjects(QStringList &pathsList);

    QString getObjectInfo(QDBusInterface &iface);
    QString getObjectCategory(QDBusInterface &iface);

    std::unique_ptr<ACModel> buildModelFromACObjects(std::vector<std::unique_ptr<ACObject>> objects);

    ACObjectItem *createCategoryItem(QString name, ACObjectCategory *nameTranslations);

private:
    QDBusConnection m_dbusConnection;
    QString m_dbusServiceName;
    QString m_dbusPath;
    QString m_managerInterface;
    QString m_dbusFindInterface;
    QString m_getObjectMethodName;
    QString m_infoMethodName;
    QString m_categoryMethonName;
};

#endif // ACOBJECTSMODELBUILDER_H
