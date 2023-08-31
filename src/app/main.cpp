#include "../core/logger/prelude.h"
#include "accontroller.h"
#include "mainwindow.h"
#include "model/aclocalapllicationmodelbuilder.h"
#include "model/acmodel.h"
#include "model/acobjectsmodelbuilder.h"

#include <QApplication>
#include <QStandardItemModel>
#include <QTranslator>

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

int main(int argc, char *argv[])
{
    auto loggerManager = ab::logger::LoggerManager::globalInstance();

    loggerManager->addLogger<ab::logger::ConsoleLogger>(QtDebugMsg);
    loggerManager->addLogger<ab::logger::FileLogger>(QtWarningMsg);
    loggerManager->addLogger<ab::logger::SyslogLogger>(LOG_LEVEL_DISABLED);

    QApplication app(argc, argv);

    QLocale locale;
    QTranslator translator;
    QString language = locale.system().name().split("_").at(0);
    translator.load(language, ".");
    app.installTranslator(&translator);

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

    std::unique_ptr<ACModel> model = objectModelBuilder.buildModel(appModel.get());
    model->translateModel(language);

    MainWindow mainWindow;

    ACController controller(&mainWindow, std::move(model));
    mainWindow.setController(&controller);

    mainWindow.show();

    return app.exec();
}
