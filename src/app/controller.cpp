#include "controller.h"
#include "../aobuilder/constants.h"
#include "mainwindow.h"
#include "model/modelinterface.h"

#include <QAction>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>

namespace ab
{
class ControllerPrivate
{
public:
    MainWindow *window{nullptr};
    std::unique_ptr<model::ModelInterface> model{nullptr};
};

Controller::Controller(MainWindow *w, std::unique_ptr<model::Model> m, QObject *parent)
    : QObject{parent}
    , d(new ControllerPrivate)
{
    d->window = w;
    d->model  = std::move(m);

    if (d->model != nullptr)
    {
        w->setModel(d->model.get());
    }

    auto alteratorWatcher = new QDBusServiceWatcher(ao_builder::DBUS_SERVICE_NAME,
                                                    QDBusConnection::systemBus(),
                                                    QDBusServiceWatcher::WatchForOwnerChange,
                                                    this);
    //    connect(alteratorWatcher, &QDBusServiceWatcher::serviceOwnerChanged, this, &Controller::onDBusStructureUpdate);
}

Controller::~Controller()
{
    delete d;
}

void Controller::moduleClicked(ao_builder::LegacyObject obj)
{
    auto apps = d->model->getLocalApplicationsByInterface(ao_builder::DBUS_LEGACY_OBJECT_INTERFACE_NAME);
    if (apps.empty())
    {
        qWarning() << obj.m_id << ": no applications are available for this module";
    }
    auto app  = d->model->getLocalApplication(apps[0]);
    auto proc = new QProcess(this);
    proc->start("/bin/bash", QStringList() << "-c" << app->m_exec << obj.m_dbus_path);
    qWarning() << proc->readAllStandardError();
    qWarning() << proc->readAllStandardOutput();
}

//void Controller::onDBusStructureUpdate(QString, QString, QString)
//{
//    model::ObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME,
//                                                  DBUS_PATH,
//                                                  DBUS_MANAGER_INTERFACE_NAME,
//                                                  DBUS_FIND_INTERFACE_NAME,
//                                                  GET_OBJECTS_METHOD_NAME,
//                                                  INFO_METHOD_NAME_FOR_ACOBJECT,
//                                                  CATEGORY_INTERFACE_NAME_FOR_ACOBJECT,
//                                                  CATEGORY_METHOD_NAME_FOR_ACOBJECT,
//                                                  DBUS_LOCAL_APP_INTERFACE_NAME,
//                                                  DBUS_LOCAL_APP_GET_LIST_OF_FILES,
//                                                  DBUS_LOCAL_APP_GET_DESKTOP_FILE);

//    std::unique_ptr<model::Model> objectModel = objectModelBuilder.buildModel();

//    d->model = std::move(objectModel);

//    QLocale locale;
//    QString language = locale.system().name().split("_").at(0);
//    d->model->translateModel(language);

//    d->window->clearUi();
//    d->window->setModel(d->model.get());
//}
} // namespace ab
