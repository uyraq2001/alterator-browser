#include "controller.h"
#include "mainwindow.h"
#include "model/localapllicationmodelbuilder.h"
#include "model/objectsmodelbuilder.h"

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

Controller::Controller(MainWindow *w, std::unique_ptr<model::Model> m, QObject *parent)
    : QObject{parent}
    , window(w)
    , model(std::move(m))
{
    if (this->model != nullptr)
    {
        w->setModel(model.get());
    }

    auto alteratorWatcher = new QDBusServiceWatcher(DBUS_SERVICE_NAME,
                                                    QDBusConnection::systemBus(),
                                                    QDBusServiceWatcher::WatchForOwnerChange,
                                                    this);
    connect(alteratorWatcher, &QDBusServiceWatcher::serviceOwnerChanged, this, &Controller::onDBusStructureUpdate);
}

void Controller::moduleClicked(model::ObjectItem *moduleItem)
{
    if (moduleItem->m_object->m_isLegacy)
    {
        auto proc = new QProcess(this);
        proc->start("alterator-standalone", QStringList() << "-l" << moduleItem->m_object.get()->m_icon);
    }
    else
    {
        window->showModuleMenu(moduleItem);
    }
}

void Controller::onInterfaceClicked(model::LocalApplication *app)
{
    auto proc = new QProcess(this);
    proc->start(app->m_desktopExec, QStringList());
}

void Controller::onDBusStructureUpdate(QString, QString, QString)
{
    model::LocalApllicationModelBuilder appModelBuilder(DBUS_SERVICE_NAME,
                                                        DBUS_LOCAL_APP_PATH,
                                                        DBUS_LOCAL_APP_INTERFACE_NAME,
                                                        DBUS_LOCAL_APP_GET_LIST_OF_FILES,
                                                        DBUS_LOCAL_APP_GET_DESKTOP_FILE);

    std::unique_ptr<model::LocalApplicationModel> appModel = appModelBuilder.buildModel();

    model::ObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME,
                                                  DBUS_PATH,
                                                  DBUS_MANAGER_INTERFACE_NAME,
                                                  DBUS_FIND_INTERFACE_NAME,
                                                  GET_OBJECTS_METHOD_NAME,
                                                  INFO_METHOD_NAME_FOR_ACOBJECT,
                                                  CATEGORY_INTERFACE_NAME_FOR_ACOBJECT,
                                                  CATEGORY_METHOD_NAME_FOR_ACOBJECT);

    std::unique_ptr<model::Model> objectModel = objectModelBuilder.buildModel(appModel.get());

    this->model = std::move(objectModel);

    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    this->model->translateModel(language);

    this->window->clearUi();
    this->window->setModel(this->model.get());
}
} // namespace ab
