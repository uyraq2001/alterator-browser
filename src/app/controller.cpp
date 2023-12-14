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
        translateModel();
    }

    auto alteratorWatcher = new QDBusServiceWatcher(ao_builder::DBUS_SERVICE_NAME,
                                                    QDBusConnection::systemBus(),
                                                    QDBusServiceWatcher::WatchForOwnerChange,
                                                    this);
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
        return;
    }
    auto app     = d->model->getLocalApplication(apps[0]);
    auto proc    = new QProcess(this);
    QString exec = app->m_exec;
    exec.replace("%o", obj.m_dbus_path);
    proc->start("/bin/bash", QStringList() << "-c" << exec);
}

void Controller::translateModel()
{
    QLocale locale;
    QString language = locale.system().name().split("_").at(0);
    d->model->translateModel(language);

    d->window->clearUi();
    if (d->model != nullptr)
    {
        d->window->setModel(d->model.get());
    }
}
} // namespace ab
