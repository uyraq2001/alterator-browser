/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "adtapp.h"
#include "../core/treemodelbulderfromexecutable.h"
#include "adtbuilderstrategies/adtmodelbuilder.h"
#include "adtbuilderstrategies/adtmodelbuilderstrategydbusinfodesktop.h"
#include "clcontroller.h"
#include "interfaces/appcontrollerinterface.h"
#include "mainwindowcontrollerimpl.h"
#include "parser/commandlineoptions.h"
#include "parser/commandlineparser.h"

#include <iostream>
#include <memory>

const QString DBUS_SERVICE_NAME         = "ru.basealt.alterator";
const QString PATH_TO_DBUS_OBJECT       = "/ru/basealt/alterator";
const QString DBUS_INTERFACE_NAME       = "ru.basealt.alterator.manager";
const QString DBUS_GET_METHOD_NAME      = "get_objects";
const QString DBUS_FIND_INTERFACE       = "ru.basealt.alterator.diag1";
const QString DBUS_RUN_TASK_METHOD_NAME = "run";

class ADTAppPrivate
{
public:
    ADTAppPrivate(QApplication *app)
        : m_application(app)
        , m_model(nullptr)
        , m_parser(new CommandLineParser(*app))
        , m_options(new CommandLineOptions())
        , m_parseErrorMessage{}
        , m_appController(nullptr)
        , m_parserResult{}
    {}

    QApplication *m_application;
    std::unique_ptr<TreeModel> m_model;
    std::unique_ptr<CommandLineParser> m_parser;
    std::unique_ptr<CommandLineOptions> m_options;
    QString m_parseErrorMessage;
    std::unique_ptr<AppControllerInterface> m_appController;

    CommandLineParser::CommandLineParseResult m_parserResult;

private:
    ADTAppPrivate(const ADTAppPrivate &) = delete;
    ADTAppPrivate(ADTAppPrivate &&)      = delete;
    ADTAppPrivate &operator=(const ADTAppPrivate &) = delete;
    ADTAppPrivate &operator=(ADTAppPrivate &&) = delete;
};

ADTApp::ADTApp(QApplication *application)
    : d(new ADTAppPrivate(application))
{}

ADTApp::~ADTApp()
{
    delete d;
}

int ADTApp::runApp()
{
    d->m_parserResult = d->m_parser->parseCommandLine(d->m_options.get(), &d->m_parseErrorMessage);

    if (d->m_parserResult == CommandLineParser::CommandLineParseResult::CommandLineError)
    {
        std::cerr << d->m_parseErrorMessage.toStdString() << std::endl;
        d->m_parser->showHelp();
        return 1;
    }

    if (d->m_parserResult == CommandLineParser::CommandLineParseResult::CommandLineHelpRequested)
    {
        d->m_parser->showHelp();
        return 0;
    }

    if (d->m_parserResult == CommandLineParser::CommandLineParseResult::CommandLineVersionRequested)
    {
        d->m_parser->showVersion();
        return 0;
    }

    buildModel();

    if (d->m_parserResult == CommandLineParser::CommandLineParseResult::CommandLineOk
        || d->m_options->useGraphic == true)
    {
        //use GUI
        d->m_appController = std::make_unique<MainWindowControllerImpl>(d->m_model.get(),
                                                                        d->m_options.get(),
                                                                        d->m_application);
    }
    else
    {
        //use CLI
        d->m_appController = std::make_unique<CLController>(d->m_model.get(), d->m_options.get());
    }

    return d->m_appController->runApp();
}

void ADTApp::buildModel()
{
    ADTModelBuilder modelBuilder(new ADTModelBuilderStrategyDbusInfoDesktop(DBUS_SERVICE_NAME,
                                                                            PATH_TO_DBUS_OBJECT,
                                                                            DBUS_INTERFACE_NAME,
                                                                            DBUS_GET_METHOD_NAME,
                                                                            DBUS_FIND_INTERFACE,
                                                                            DBUS_RUN_TASK_METHOD_NAME,
                                                                            new TreeModelBulderFromExecutable()));
    d->m_model = std::move(modelBuilder.buildModel());
}
