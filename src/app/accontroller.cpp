#include "accontroller.h"

#include <QAction>
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QDBusServiceWatcher>
#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include "model/acobjectsmodelbuilder.h"
#include "model/aclocalapllicationmodelbuilder.h"

const QString DBUS_SERVICE_NAME                    = "ru.basealt.alterator";
const QString DBUS_PATH                            = "/ru/basealt/alterator";
const QString DBUS_FIND_INTERFACE_NAME             = "ru.basealt.alterator.object";
const QString DBUS_MANAGER_INTERFACE_NAME          = "ru.basealt.alterator.manager";
const QString GET_OBJECTS_METHOD_NAME              = "get_objects";
const QString INFO_METHOD_NAME_FOR_ACOBJECT        = "info";
const QString CATEGORY_INTERFACE_NAME_FOR_ACOBJECT = "ru.basealt.alterator.categories";
const QString CATEGORY_METHOD_NAME_FOR_ACOBJECT    = "info";

const QString DBUS_LOCAL_APP_PATH              = "/ru/basealt/alterator/applications";
const QString DBUS_LOCAL_APP_INTERFACE_NAME    = "ru.basealt.alterator.applications";
const QString DBUS_LOCAL_APP_GET_LIST_OF_FILES = "list";
const QString DBUS_LOCAL_APP_GET_DESKTOP_FILE  = "info";

ACController::ACController(MainWindow *w, ACModel *m, QObject *parent)
    : QObject{parent}
    , window(w)
    , model(m)
{
    if (m){
        w->setModel(model.get());
    }

//    QDBusServiceWatcher alteratorWatcher("ru.basealt.alterator",
//                                         QDBusConnection::systemBus(),
//                                         QDBusServiceWatcher::WatchForOwnerChange);
//    connect(&alteratorWatcher, &QDBusServiceWatcher::serviceOwnerChanged,
////            this, [](QString a, ){qWarning() << "dxfcgvhbjnkl;" << a;});
//            this, &ACController::onDBusStructureUpdate);

//    connect(QDBusConnection::sessionBus().interface(), &QDBusConnectionInterface::serviceRegistered, this, [=](QString a) {
//                qDebug() <<"service R: " << a;
//            });
//    connect(&alteratorWatcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
//            this, SLOT(onDBusStructureUpdate(QString,QString,QString)));

    QDBusConnection::systemBus().connect("org.freedesktop.DBus",
                                         "/org/freedesktop/DBus",
                                         "org.freedesktop.DBus",
                                         "NameOwnerChanged",
                                         QStringList() << "ru.basealt.alterator", "sss",
                                         this, SLOT(onDBusStructureUpdate(QString,QString,QString)));
}


ACController::~ACController() {}

void ACController::moduleClicked(ACObjectItem *moduleItem)
{
    if (moduleItem->m_acObject->m_isLegacy){
        QProcess *proc = new QProcess(this);
        proc->start("alterator-standalone", QStringList() << "-l" << moduleItem->m_acObject.get()->m_icon);
    }else{
        window->showModuleMenu(moduleItem);
    }
}

void ACController::onInterfaceClicked(ACLocalApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_desktopExec, QStringList());
}

void ACController::onDBusStructureUpdate(QString service, QString prev, QString next)
{
    ACLocalApllicationModelBuilder appModelBuilder(DBUS_SERVICE_NAME,
                                                   DBUS_LOCAL_APP_PATH,
                                                   DBUS_LOCAL_APP_INTERFACE_NAME,
                                                   DBUS_LOCAL_APP_GET_LIST_OF_FILES,
                                                   DBUS_LOCAL_APP_GET_DESKTOP_FILE);

    std::unique_ptr<ACLocalApplicationModel> appModel = appModelBuilder.buildModel();

    ACObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME,
                                             DBUS_PATH,
                                             DBUS_MANAGER_INTERFACE_NAME,
                                             DBUS_FIND_INTERFACE_NAME,
                                             GET_OBJECTS_METHOD_NAME,
                                             INFO_METHOD_NAME_FOR_ACOBJECT,
                                             CATEGORY_INTERFACE_NAME_FOR_ACOBJECT,

                                             CATEGORY_METHOD_NAME_FOR_ACOBJECT);

    std::unique_ptr<ACModel> objModel = objectModelBuilder.buildModel(appModel.get());

    if (!objModel)
    {
        return;
    }

    model = std::move(objModel);
    model->translateModel(QString("ru"));
    window->clearUi();
    window->setModel(model.get());
}
