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

#include "clcontroller.h"
#include "../core/treeitem.h"
#include "adtexecutor.h"

#include <iostream>

class CLControllerPrivate
{
public:
    CLControllerPrivate(TreeModel *model, CommandLineOptions *options)
        : m_options(options)
        , m_model(model)
        , m_executor(new ADTExecutor())
    {}
    ~CLControllerPrivate() { delete m_executor; }

    CommandLineOptions *m_options;
    TreeModel *m_model;
    ADTExecutor *m_executor;

private:
    CLControllerPrivate(const CLControllerPrivate &) = delete;
    CLControllerPrivate(CLControllerPrivate &&)      = delete;
    CLControllerPrivate &operator=(const CLControllerPrivate &) = delete;
    CLControllerPrivate &operator=(CLControllerPrivate &&) = delete;
};

CLController::CLController(TreeModel *model, CommandLineOptions *options)
    : d(new CLControllerPrivate(model, options))
{
    connect(d->m_executor, &ADTExecutor::beginTask, this, &CLController::onBeginTask);
    connect(d->m_executor, &ADTExecutor::finishTask, this, &CLController::onFinishTask);
    connect(d->m_executor, &ADTExecutor::allTaskBegin, this, &CLController::onAllTasksBegin);
    connect(d->m_executor, &ADTExecutor::allTasksFinished, this, &CLController::onAllTasksFinished);
}

CLController::~CLController()
{
    delete d;
}

int CLController::listObjects()
{
    TreeItem *rootItem = static_cast<TreeItem *>(d->m_model->parent(QModelIndex()).internalPointer());

    if (!rootItem)
    {
        std::cerr << "ERROR: can't get root item!" << std::endl;
        return 1;
    }

    if (rootItem->childCount() < 1)
    {
        std::cerr << "There is no object!" << std::endl;
        return 1;
    }

    for (int i = 0; i < rootItem->childCount(); i++)
    {
        std::cout << rootItem->child(i)->getExecutable()->m_id.toStdString() << std::endl;
    }

    return 0;
}

int CLController::listTestsOfObject(QString object)
{
    TreeItem *currentTool = getToolById(object);

    if (!currentTool || currentTool->childCount() < 1)
    {
        std::cerr << "ERROR: can't find tests in object: " << object.toStdString() << std::endl;
        return 1;
    }

    for (int i = 0; i < currentTool->childCount(); i++)
    {
        std::cout << currentTool->child(i)->getExecutable()->m_id.toStdString() << std::endl;
    }

    return 0;
}

int CLController::runAllTestsOfObject(QString object)
{
    TreeItem *currentTool = getToolById(object);

    if (!currentTool || currentTool->childCount() < 1)
    {
        std::cerr << "ERROR: can't find tests in object: " << object.toStdString() << std::endl;
        return 1;
    }

    std::vector<ADTExecutable *> tasks;
    for (int i = 0; i < currentTool->childCount(); i++)
    {
        tasks.push_back(currentTool->child(i)->getExecutable());
    }

    if (!tasks.empty())
    {
        d->m_executor->resetStopFlag();
        d->m_executor->setTasks(tasks);
        d->m_executor->runTasks();
    }

    return 0;
}

int CLController::runSpecifiedTestOfObject(QString object, QString test)
{
    TreeItem *currentTool = getToolById(object);

    if (!currentTool || currentTool->childCount() < 1)
    {
        std::cerr << "ERROR: can't find tests in object: " << object.toStdString() << std::endl;
        return 1;
    }

    std::vector<ADTExecutable *> tasks;
    for (int i = 0; i < currentTool->childCount(); i++)
    {
        if (currentTool->child(i)->getExecutable()->m_id == test)
        {
            tasks.push_back(currentTool->child(i)->getExecutable());
        }
    }

    if (!tasks.empty())
    {
        d->m_executor->resetStopFlag();
        d->m_executor->setTasks(tasks);
        d->m_executor->runTasks();
    }

    return 0;
}

int CLController::runApp()
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

    return result;
}

TreeItem *CLController::getToolById(QString id)
{
    TreeItem *rootItem = static_cast<TreeItem *>(d->m_model->parent(QModelIndex()).internalPointer());

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

void CLController::onAllTasksBegin() {}

void CLController::onAllTasksFinished() {}

void CLController::onBeginTask(ADTExecutable *task)
{
    std::cout << "Running test: " << task->m_id.toStdString() << "...";
}

void CLController::onFinishTask(ADTExecutable *task)
{
    if (task->m_exit_code == 0)
    {
        std::cout << "OK" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
}
