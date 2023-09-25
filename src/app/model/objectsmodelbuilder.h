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
                        QString categoryMethodName,
                        QString interfaceName,
                        QString getListOfFilesMethod,
                        QString getDesktopFileMethod);

    std::unique_ptr<Model> buildModel();

private:
    void mergeApplicationModel(Model *objectModel, LocalApplicationModel *appModel);
    void mergeObjectWithApp(ObjectItem *item, LocalApplicationModel *appModel);

    QStringList getListOfObjects();
    std::vector<std::unique_ptr<std::variant<Object, Category>>> parseObjects(QStringList &pathsList);
    QString getObjectInfo(QDBusInterface &iface);
    std::unique_ptr<Model> buildModelFromObjects(std::vector<std::unique_ptr<std::variant<Object, Category>>> objects);
    ObjectItem *createCategoryItem(QString categoryName);

private:
    QDBusConnection m_dbusConnection = QDBusConnection::systemBus();
    QString m_dbusServiceName{};
    QString m_dbusPath{};
    QString m_managerInterface{};
    QString m_dbusFindInterface{};
    QString m_getObjectMethodName{};
    QString m_infoMethodName{};
    QString m_categoryInterfaceName{};
    QString m_categoryMethodName{};

    QString m_interface{};
    QString m_getFilesMethodName{};
    QString m_getDesktopFileMethodName{};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECTS_MODEL_BUILDER_H
