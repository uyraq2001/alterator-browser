#include "controller.h"
#include "../core/logger/prelude.h"
#include "mainwindow.h"
#include "model/modelinterface.h"
#include <memory>

#include <utility>
#include <vector>
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
    std::shared_ptr<MainWindow> window{nullptr};
    std::unique_ptr<model::ModelInterface> model{nullptr};
    std::unique_ptr<ao_builder::DataSourceInterface> dataSource{nullptr};
    std::unique_ptr<ao_builder::AOBuilderInterface> modelBuilder{nullptr};
    std::vector<QString> interfaces{"ru.basealt.alterator.legacy",
                                    "ru.basealt.alterator.object",
                                    "ru.basealt.alterator.diag1",
                                    "ru.basealt.alterator.test_interface1",
                                    "ru.basealt.alterator.test_interface2"};

    ControllerPrivate(std::shared_ptr<MainWindow> w,
                      std::unique_ptr<model::ModelInterface> m,
                      std::unique_ptr<ao_builder::DataSourceInterface> ds,
                      std::unique_ptr<ao_builder::AOBuilderInterface> mb)
        : window(std::move(w))
        , model(std::move(m))
        , dataSource(std::move(ds))
        , modelBuilder(std::move(mb))
    {}
};

Controller::Controller(std::shared_ptr<MainWindow> w,
                       std::unique_ptr<model::ModelInterface> m,
                       std::unique_ptr<ao_builder::DataSourceInterface> ds,
                       std::unique_ptr<ao_builder::AOBuilderInterface> mb,
                       QObject *parent)
    : QObject{parent}
    , d{new ControllerPrivate(std::move(w), std::move(m), std::move(ds), std::move(mb))}
{
    buildModel();

    if (d->model == nullptr)
    {
        qCritical() << "Can not build model";
        return;
    }

    d->window->setModel(d->model.get());
    translateModel();
}

Controller::~Controller()
{
    delete d;
}

void Controller::moduleClicked(ao_builder::Object *object)
{
    auto apps = d->model->getLocalApplicationsByInterface(object->m_interface);
    if (apps.empty())
    {
        qWarning() << object->m_id << ": no applications are available for this module";
        return;
    }
    auto app     = d->model->getLocalApplication(apps[0]);
    auto proc    = new QProcess(this);
    QString exec = app->m_exec;
    exec.replace("%o", object->m_dbus_path);
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

void Controller::buildModel()
{
    auto categories = d->modelBuilder->buildCategories();

    auto apps = d->modelBuilder->buildLocalApps();

    auto objects = d->modelBuilder->buildObjects(d->interfaces);

    d->model->build(std::move(categories), std::move(apps), std::move(objects));
}

void Controller::switchBack()
{
    auto proc = new QProcess(this);
    proc->start("acc", QStringList());
}

} // namespace ab
