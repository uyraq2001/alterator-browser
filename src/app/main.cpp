#include "../core/logger/prelude.h"
#include "controller.h"
#include "mainwindow.h"
#include "model/model.h"

#include <iostream>
#include <memory>
#include <QApplication>
#include <QDebug>
#include <QStandardItemModel>
#include <QTranslator>

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

    auto model = std::make_unique<ab::model::Model>();

    ab::MainWindow mainWindow;
    ab::Controller controller(&mainWindow, std::move(model));
    mainWindow.setController(&controller);
    mainWindow.show();

    return app.exec();
}
