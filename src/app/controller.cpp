#include "controller.h"
#include "mainwindow.h"
#include "model/constants.h"
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

    auto alteratorWatcher = new QDBusServiceWatcher(DBUS_SERVICE_NAME,
                                                    QDBusConnection::systemBus(),
                                                    QDBusServiceWatcher::WatchForOwnerChange,
                                                    this);
    connect(alteratorWatcher, &QDBusServiceWatcher::serviceOwnerChanged, this, &Controller::onDBusStructureUpdate);
}

Controller::~Controller()
{
    delete d;
}

void Controller::moduleClicked(model::ObjectItem *moduleItem)
{
    try
    {
        if (std::get<ab::model::Object>(*(moduleItem->m_object)).m_isLegacy)
        {
            QProcess *proc = new QProcess();

            connect(proc, &QProcess::readyReadStandardError, this, [proc]() {
                qCritical() << proc->readAllStandardError();
            });
            connect(proc, &QProcess::readyReadStandardOutput, this, [proc]() {
                qInfo() << proc->readAllStandardOutput();
            });
            connect(proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [proc](int) {
                delete (proc);
            });

            proc->start("alterator-standalone",
                        QStringList() << "-l"
                                      << std::get<ab::model::Object>(*moduleItem->m_object).m_x_Alterator_Internal_Name);
        }
        else
        {
            auto ifaces = std::get<ab::model::Object>(*moduleItem->m_object).m_interfaces;
            if (ifaces.size() == 1)
            {
                onInterfaceClicked(ifaces[0]);
            }
        }
    }
    catch (const std::bad_variant_access &e)
    {
        qCritical() << "ERROR: the item is not of Object type";
    }
}

void Controller::onInterfaceClicked(QString &iface)
{
    auto apps = d->model->appModel->getAppsByInterface(iface);
    auto proc = new QProcess(this);
    proc->start("/bin/bash", QStringList() << "-c" << apps[0]->m_desktopExec);
    qWarning() << proc->readAllStandardError() << proc->readAllStandardOutput();
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
