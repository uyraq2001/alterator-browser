#ifndef ACOBJECTSMODELBUILDER_H
#define ACOBJECTSMODELBUILDER_H

#include "localapplicationmodel.h"
#include "model.h"
#include "object.h"
#include "objectitem.h"

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
                          QString categoryInterfaceName,
                          QString categoryMethodName);

    std::unique_ptr<ACModel> buildModel(ACLocalApplicationModel *appModel);

private:
    void mergeApplicationModel(ACModel *objectModel, ACLocalApplicationModel *appModel);

    void mergeObjectWithApp(ACObjectItem *item, ACLocalApplicationModel *appModel);

    QStringList getListOfACObjects();

    std::vector<std::unique_ptr<ACObject>> parseACObjects(QStringList &pathsList);

    QString getObjectInfo(QDBusInterface &iface);

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
    QString m_categoryInterfaceName;
    QString m_categoryMethodName;
};

#endif // ACOBJECTSMODELBUILDER_H
