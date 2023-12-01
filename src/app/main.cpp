#include "../core/logger/prelude.h"
/* #include "controller.h" */
/* #include "mainwindow.h" */
#include "model/constants.h"
#include "model/localapllicationmodelbuilder.h"
#include "model/model.h"
/* #include "model/objectsmodelbuilder.h" */
#include <iostream>
#include <memory>

#include <QApplication>
#include <QStandardItemModel>
#include <QTranslator>
#include <qdebug.h>
#include <qstandarditemmodel.h>

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

    /* ab::model::ObjectsModelBuilder objectModelBuilder(DBUS_SERVICE_NAME, */
    /*                                                   DBUS_PATH, */
    /*                                                   DBUS_MANAGER_INTERFACE_NAME, */
    /*                                                   DBUS_FIND_INTERFACE_NAME, */
    /*                                                   GET_OBJECTS_METHOD_NAME, */
    /*                                                   INFO_METHOD_NAME_FOR_ACOBJECT, */
    /*                                                   CATEGORY_INTERFACE_NAME_FOR_ACOBJECT, */
    /*                                                   CATEGORY_METHOD_NAME_FOR_ACOBJECT, */
    /*                                                   DBUS_LOCAL_APP_INTERFACE_NAME, */
    /*                                                   DBUS_LOCAL_APP_GET_LIST_OF_FILES, */
    /*                                                   DBUS_LOCAL_APP_GET_DESKTOP_FILE); */

    auto model      = std::make_unique<ab::model::Model>();
    auto categories = model->getCategories();

    for (const auto &category : categories)
    {
        std::cout << category.toStdString() << std::endl;
    }

    /* auto root           = model->invisibleRootItem(); */
    /* auto categoriesRoot = std::make_unique<QStandardItem>(); */
    /* for (const auto &category : categories) */
    /* { */
    /*     auto item = std::make_unique<QStandardItem>(); */
    /*     item->setData(QVariant::fromValue(category.get())); */
    /*     categoriesRoot->appendRow(item.release()); */
    /*     qWarning() << "appended item" << category->m_displayName; */
    /* } */
    /* root->appendRow(categoriesRoot.release()); */

    /* std::unique_ptr<ab::model::Model> model = objectModelBuilder.buildModel(); */

    /* ab::MainWindow mainWindow; */

    /* ab::Controller controller(&mainWindow, std::move(model)); */

    /* mainWindow.setController(&controller); */

    /* mainWindow.show(); */

    return app.exec();
}
