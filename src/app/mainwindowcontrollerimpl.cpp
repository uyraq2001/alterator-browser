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

#include "mainwindowcontrollerimpl.h"
#include "mainwindow/mainwindow.h"

#include <QThread>

class MainWindowControllerImplPrivate
{
public:
    MainWindowControllerImplPrivate(TreeModel *model, CommandLineOptions *options, QApplication *app)
        : m_model(model)
        , m_mainWindow(nullptr)
        , m_executor(new ADTExecutor())
        , m_workerThread(nullptr)
        , m_isWorkingThreadActive(false)
        , m_options(options)
        , m_application(app)
    {
        m_mainWindow  = new MainWindow();

    }
    ~MainWindowControllerImplPrivate()
    {
        delete m_mainWindow;
    }

    TreeModel *m_model;

    MainWindowInterface *m_mainWindow;

    std::unique_ptr<ADTExecutor> m_executor;

    QThread *m_workerThread;

    bool m_isWorkingThreadActive;

    CommandLineOptions *m_options;

    QApplication *m_application;

private:
    MainWindowControllerImplPrivate(const MainWindowControllerImplPrivate &) = delete;
    MainWindowControllerImplPrivate(MainWindowControllerImplPrivate &&)      = delete;
    MainWindowControllerImplPrivate &operator=(const MainWindowControllerImplPrivate &) = delete;
    MainWindowControllerImplPrivate &operator=(MainWindowControllerImplPrivate &&) = delete;
};

MainWindowControllerImpl::MainWindowControllerImpl(TreeModel *model, CommandLineOptions *options, QApplication *app)
    : d(new MainWindowControllerImplPrivate(model, options, app))
{
    d->m_mainWindow->setController(this);

    connect(d->m_executor.get(), &ADTExecutor::beginTask, this, &MainWindowControllerImpl::onBeginTask);
    connect(d->m_executor.get(), &ADTExecutor::finishTask, this, &MainWindowControllerImpl::onFinishTask);
    connect(d->m_executor.get(), &ADTExecutor::allTaskBegin, this, &MainWindowControllerImpl::onAllTasksBegin);
    connect(d->m_executor.get(), &ADTExecutor::allTasksFinished, this, &MainWindowControllerImpl::onAllTasksFinished);
}

MainWindowControllerImpl::~MainWindowControllerImpl()
{
    delete d;
}

void MainWindowControllerImpl::runAllToolsWidget()
{
    d->m_mainWindow->toggleStackWidget();
}

void MainWindowControllerImpl::chooseToolsWidget()
{
    d->m_mainWindow->toggleStackWidget();
}

void MainWindowControllerImpl::exitToolsWidget()
{
    d->m_mainWindow->closeAll();
}

void MainWindowControllerImpl::changeSelectedTool(TreeItem *item)
{
    if (!item)
    {
        return;
    }
}

void MainWindowControllerImpl::runTestsWidget(std::vector<ADTExecutable *> tasks)
{
    d->m_executor->setTasks(tasks);

    d->m_executor->resetStopFlag();

    d->m_workerThread = new QThread();

    connect(d->m_workerThread, &QThread::started, d->m_executor.get(), &ADTExecutor::runTasks);
    connect(d->m_workerThread, &QThread::finished, d->m_workerThread, &QObject::deleteLater);

    d->m_executor->moveToThread(d->m_workerThread);

    d->m_workerThread->start();
}

void MainWindowControllerImpl::backTestsWigdet()
{
    d->m_mainWindow->toggleStackWidget();
}

void MainWindowControllerImpl::exitTestsWidget()
{
    d->m_mainWindow->closeAll();
}

void MainWindowControllerImpl::detailsCurrentTest(ADTExecutable *test)
{}

int MainWindowControllerImpl::listObjects()
{
    return 0;
}

int MainWindowControllerImpl::listTestsOfObject(QString object)
{
    TreeItem *toolItem = getToolById(object);
    if (!toolItem)
    {
        qWarning() << "ERROR: can't find tool with id: " + object;
        //TO DO show messagebox with warning
        return 1;
    }

    changeSelectedTool(toolItem);
    d->m_mainWindow->toggleStackWidget();

    return 0;
}

int MainWindowControllerImpl::runAllTestsOfObject(QString object)
{
    TreeItem *toolItem = getToolById(object);
    if (!toolItem)
    {
        qWarning() << "ERROR: can't find tool with id: " + object;
        //TO DO show messagebox with warning
        return 1;
    }

    changeSelectedTool(toolItem);
    d->m_mainWindow->toggleStackWidget();

    return 0;
}

int MainWindowControllerImpl::runSpecifiedTestOfObject(QString object, QString test)
{
    TreeItem *toolItem = getToolById(object);
    if (!toolItem)
    {
        qWarning() << "ERROR: can't find tool with id: " + object;
        //TO DO show messagebox with warning
        return 1;
    }

    changeSelectedTool(toolItem);
    d->m_mainWindow->toggleStackWidget();

    ADTExecutable *runningTest = nullptr;

    if (!runningTest)
    {
        qWarning() << "ERROR: can't find test with id: " + test + " in tool: " + object;
        //TO DO show messagebox with warning
        return 1;
    }

    runTestsWidget(std::vector<ADTExecutable *>{runningTest});
    return 0;
}

int MainWindowControllerImpl::runApp()
{
    int result = -1;

    switch (d->m_options->action)
    {
    case CommandLineOptions::Action::listOfObjects:
        result = listObjects();
        break;
    case CommandLineOptions::Action::listOfTestFromSpecifiedObject:
        result = listTestsOfObject(d->m_options->objectName);
        break;
    case CommandLineOptions::Action::runAllTestFromSpecifiedObject:
        result = runAllTestsOfObject(d->m_options->objectName);
        break;
    case CommandLineOptions::Action::runSpecifiedTestFromSpecifiedObject:
        result = runSpecifiedTestOfObject(d->m_options->objectName, d->m_options->testName);
        break;
    default:
        break;
    }

    auto mainWindow = dynamic_cast<MainWindow *>(d->m_mainWindow);

    mainWindow->show();

    d->m_application->exec();

    return result;
}

void MainWindowControllerImpl::clearAllReports()
{
    QModelIndex rootIndex = d->m_model->parent(QModelIndex());

    TreeItem *rootItem = static_cast<TreeItem *>(rootIndex.internalPointer());

    if (!rootItem)
    {
        qWarning() << "ERROR: can't get root item to clear all reports!";

        return;
    }

    for (int i = rootItem->childCount() - 1; i >= 0; i--)
    {
        clearToolReports(rootItem->child(i));
    }
}

void MainWindowControllerImpl::clearToolReports(TreeItem *item)
{
    if (item->childCount() == 0)
    {
        return;
    }

    for (int i = item->childCount() - 1; i >= 0; i--)
    {
        TreeItem *currentChild = item->child(i);

        currentChild->getExecutable()->clearReports();
    }
}

void MainWindowControllerImpl::onAllTasksBegin()
{
    d->m_isWorkingThreadActive = true;
}

void MainWindowControllerImpl::onAllTasksFinished()
{
    d->m_isWorkingThreadActive = false;
}

void MainWindowControllerImpl::onBeginTask(ADTExecutable *task)
{
}

void MainWindowControllerImpl::onFinishTask(ADTExecutable *task)
{
}

TreeItem *MainWindowControllerImpl::getToolById(QString id)
{
    TreeItem *rootItem = static_cast<TreeItem *>(d->m_model->parent(QModelIndex()).internalPointer());

    if (rootItem->childCount() < 1)
    {
        qWarning() << "ERROR: where is not tests in this tool!";
        return nullptr;
    }

    for (int i = 0; i < rootItem->childCount(); i++)
    {
        TreeItem *currentChild = rootItem->child(i);
        if (currentChild->getExecutable()->m_id == id)
        {
            return currentChild;
        }
    }

    return nullptr;
}
