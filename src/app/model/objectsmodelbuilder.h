#ifndef AB_OBJECTS_MODEL_BUILDER_H
#define AB_OBJECTS_MODEL_BUILDER_H

#include "localapplicationmodel.h"
#include "model.h"
#include "object.h"
#include "objectitem.h"

#include <QDBusConnection>
#include <QStandardItemModel>

namespace ab
{
namespace model
{
class ObjectsModelBuilder
{
public:
    ObjectsModelBuilder(QString serviceName,
                        QString dbusPath,
                        QString managerIface,
                        QString findInterface,
                        QString getObjectMethodName,
                        QString infoMethodName,
                        QString categoryInterfaceName,
                        QString categoryMethodName);

    std::unique_ptr<Model> buildModel(LocalApplicationModel *appModel);

private:
    void mergeApplicationModel(Model *objectModel, LocalApplicationModel *appModel);

    void mergeObjectWithApp(ObjectItem *item, LocalApplicationModel *appModel);

    QStringList getListOfACObjects();

    std::vector<std::unique_ptr<Object>> parseACObjects(QStringList &pathsList);

    QString getObjectInfo(QDBusInterface &iface);

    std::unique_ptr<Model> buildModelFromACObjects(std::vector<std::unique_ptr<Object>> objects);

    ObjectItem *createCategoryItem(QString name, ObjectCategory *nameTranslations);

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
} // namespace model
} // namespace ab

#endif // AB_OBJECTS_MODEL_BUILDER_H
