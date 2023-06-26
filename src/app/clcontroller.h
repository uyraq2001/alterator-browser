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

#ifndef CLCONTROLLER_H
#define CLCONTROLLER_H

#include "../core/treemodel.h"
#include "interfaces/appcontrollerinterface.h"
#include "parser/commandlineoptions.h"

#include <QString>

class CLControllerPrivate;

class CLController : public AppControllerInterface
{
    Q_OBJECT
public:
    CLController(TreeModel *model, CommandLineOptions *options);
    ~CLController();

    int listObjects() override;

    int listTestsOfObject(QString object) override;

    int runAllTestsOfObject(QString object) override;

    int runSpecifiedTestOfObject(QString object, QString test) override;

    int runApp() override;

private:
    TreeItem *getToolById(QString id);

private slots:
    void onAllTasksBegin() override;
    void onAllTasksFinished() override;

    void onBeginTask(ADTExecutable *task) override;
    void onFinishTask(ADTExecutable *task) override;

private:
    CLControllerPrivate *d;

private:
    CLController(const CLController &) = delete;
    CLController(CLController &&)      = delete;
    CLController &operator=(const CLController &) = delete;
    CLController &operator=(CLController &&) = delete;
};

#endif // CLCONTROLLER_H
