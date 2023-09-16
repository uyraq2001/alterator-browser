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
class ControllerPrivate
{
public:
    MainWindow *window{nullptr};
    std::unique_ptr<model::Model> model{nullptr};
};

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
    , d(new ControllerPrivate)
{
    d->window = w;
    d->model  = std::move(m);

    if (d->model != nullptr)
    {
        w->setModel(d->model.get());
    }

    QDBusServiceWatcher *alteratorWatcher = new QDBusServiceWatcher(DBUS_SERVICE_NAME,
                                                                    QDBusConnection::systemBus(),
                                                                    QDBusServiceWatcher::WatchForOwnerChange,
                                                                    this);
    connect(alteratorWatcher, &QDBusServiceWatcher::serviceOwnerChanged, this, &Controller::onDBusStructureUpdate);
}

void Controller::moduleClicked(model::ObjectItem *moduleItem)
{
    if (moduleItem->m_object->toObject()->m_isLegacy)
    {
        QProcess *proc = new QProcess();

        connect(proc, &QProcess::readyReadStandardError, this, [proc]() {
            qCritical() << proc->readAllStandardError();
        });
        connect(proc, &QProcess::readyReadStandardOutput, this, [proc]() { qInfo() << proc->readAllStandardOutput(); });
        connect(proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [proc](int) { delete (proc); });

        proc->start("alterator-standalone", QStringList() << "-l" << moduleItem->m_object.get()->toObject()->m_icon);
    }
    else
    {
        if (moduleItem->m_object->toObject()->m_applications.size() == 1)
        {
            auto app = moduleItem->m_object->toObject()->m_applications[0];
            onInterfaceClicked(app);
            return;
        }
    }
}

void Controller::onInterfaceClicked(model::LocalApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_desktopExec, QStringList());
}

void Controller::onDBusStructureUpdate(QString, QString, QString)
{
    model::ObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME,
                                                  DBUS_PATH,
                                                  DBUS_MANAGER_INTERFACE_NAME,
                                                  DBUS_FIND_INTERFACE_NAME,
                                                  GET_OBJECTS_METHOD_NAME,
                                                  INFO_METHOD_NAME_FOR_ACOBJECT,
                                                  CATEGORY_INTERFACE_NAME_FOR_ACOBJECT,
                                                  CATEGORY_METHOD_NAME_FOR_ACOBJECT,
                                                  DBUS_LOCAL_APP_INTERFACE_NAME,
                                                  DBUS_LOCAL_APP_GET_LIST_OF_FILES,
                                                  DBUS_LOCAL_APP_GET_DESKTOP_FILE);

    std::unique_ptr<model::Model> objectModel = objectModelBuilder.buildModel();

    d->model = std::move(objectModel);

    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    d->model->translateModel(language);

    d->window->clearUi();
    d->window->setModel(d->model.get());
}
} // namespace ab
