#include "../core/logger/prelude.h"
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

    // Tests for categories

    auto categories = model->getCategories();
    std::cout << "Found categories:" << std::endl;
    for (const auto &category : categories)
    {
        std::cout << "\t" << category.toStdString() << std::endl;
    }

    std::cout << "Icon of first category: " << model->getCategory(categories.front())->m_icon.toStdString()
              << std::endl;

    // Tests for applications

    auto localApps = model->getLocalApplications();
    std::cout << "Found apps:" << std::endl;
    for (const auto &localApp : localApps)
    {
        std::cout << "\t" << localApp.toStdString() << std::endl;
    }

    std::cout << "Exec of first app: " << model->getLocalApplication(localApps.front())->m_exec.toStdString()
              << std::endl;

    auto interfaces = model->getInterfaces();
    std::cout << "Found interfaces:" << std::endl;
    for (const auto &interface : interfaces)
    {
        std::cout << "\t" << interface.toStdString() << std::endl;
    }

    auto localAppsInterface1 = model->getLocalApplicationsByInterface("interface1");
    std::cout << "Found apps with interface1:" << std::endl;
    for (const auto &localApp : localAppsInterface1)
    {
        std::cout << "\t" << localApp.toStdString() << std::endl;
    }

    // Tests for objects

    auto legacyObjects = model->getLegacyObjects();
    std::cout << "Found legacy objects:" << std::endl;
    for (const auto &object : legacyObjects)
    {
        std::cout << "\t" << object.toStdString() << std::endl;
    }

    std::cout << "Alterator URI of first legacy object: "
              << model->getLegacyObject(legacyObjects.front())->m_x_Alterator_URI.toStdString() << std::endl;

    auto legacyObjectsAboba = model->getLegacyObjectsByInterface("aboba");
    std::cout << "Found legacy objects interface \"aboba\":" << std::endl;
    for (const auto &object : legacyObjectsAboba)
    {
        std::cout << "\t" << object.toStdString() << std::endl;
    }

    auto legacyObjectsLegacy = model->getLegacyObjectsByInterface("legacy");
    std::cout << "Found legacy objects interface \"legacy\":" << std::endl;
    for (const auto &object : legacyObjectsLegacy)
    {
        std::cout << "\t" << object.toStdString() << std::endl;
    }

    auto legacyObjectsUsers = model->getLegacyObjectsByCategory("X-Alterator-Users");
    std::cout << "Found legacy objects with category \"X-Alterator-Users\":" << std::endl;
    for (const auto &object : legacyObjectsUsers)
    {
        std::cout << "\t" << object.toStdString() << std::endl;
    }

    /* ab::MainWindow mainWindow; */
    /* ab::Controller controller(&mainWindow, std::move(model)); */
    /* mainWindow.setController(&controller); */
    /* mainWindow.show(); */

    return app.exec();
}
