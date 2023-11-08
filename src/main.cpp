#include <qbase/logger/prelude.h>
#include "controller.h"
#include "mainwindow.h"
#include "model/model.h"
#include "model/objectsmodelbuilder.h"
#include "model/constants.h"

#include <QApplication>
#include <QStandardItemModel>
#include <QTranslator>

int main(int argc, char *argv[])
{
    auto loggerManager = qbase::logger::LoggerManager::globalInstance();

    loggerManager->addLogger<qbase::logger::ConsoleLogger>(QtDebugMsg);
    loggerManager->addLogger<qbase::logger::FileLogger>(QtWarningMsg,  "alterator-browser", "alterator-browser.log");
    loggerManager->addLogger<qbase::logger::SyslogLogger>(LOG_LEVEL_DISABLED, "alterator-browser");

    QApplication app(argc, argv);

    QLocale locale;
    QTranslator translator;
    QString language = locale.system().name().split("_").at(0);
    translator.load(language, ".");
    app.installTranslator(&translator);

    ab::model::ObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME,
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

    std::unique_ptr<ab::model::Model> model = objectModelBuilder.buildModel();

    ab::MainWindow mainWindow;

    ab::Controller controller(&mainWindow, std::move(model));

    mainWindow.setController(&controller);

    mainWindow.show();

    return app.exec();
}
