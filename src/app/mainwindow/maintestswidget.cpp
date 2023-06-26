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

#include "maintestswidget.h"
#include "ui_maintestswidget.h"

#include <QStyle>
#include <QThread>

const int LAYOUT_STRETCH_INDEX  = 100;
const int LAYOUT_STRETCH_FACTOR = 400;
const int LAYOUT_INDEX          = 10;

MainTestsWidget::MainTestsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainTestsWidget)
    , m_controller(nullptr)
    , m_summaryLayout(new QVBoxLayout())
    , m_detailsLayout(new QVBoxLayout())
    , m_detailsText(new QPlainTextEdit())
    , m_backToSummaryWidgetButton(new QPushButton())
    , m_currentToolItem(nullptr)
    , m_statusWidgets()
{
    ui->setupUi(this);

    ui->summaryScrollAreaWidgetContents->setLayout(m_summaryLayout);
    ui->detailsScrollAreaWidgetContents->setLayout(m_detailsLayout);

    m_backToSummaryWidgetButton->setText(tr("Back"));

    QHBoxLayout *detailsHButtonLayout = new QHBoxLayout();
    detailsHButtonLayout->addStretch();
    detailsHButtonLayout->addWidget(m_backToSummaryWidgetButton);
    connect(m_backToSummaryWidgetButton, &QPushButton::clicked, this, &MainTestsWidget::onBackToSummaryButtonClicked);

    m_detailsLayout->addWidget(m_detailsText);
    m_detailsLayout->insertLayout(LAYOUT_INDEX, detailsHButtonLayout);
    ui->detailsScrollAreaWidgetContents->setLayout(m_detailsLayout);
}

MainTestsWidget::~MainTestsWidget()
{
    delete ui;
}

void MainTestsWidget::setController(MainWindowControllerInterface *controller)
{
    m_controller = controller;
}

void MainTestsWidget::setToolItem(TreeItem *item)
{
    m_currentToolItem = item;

    clearUi();

    updateStatusWidgets();

    updateListOfStatusCommonWidgets();
}

void MainTestsWidget::enableButtons()
{
    ui->backPushButton->setEnabled(true);
    ui->exitPushButton->setEnabled(true);
    ui->runAllTestPushButton->setEnabled(true);
}

void MainTestsWidget::disableButtons()
{
    ui->backPushButton->setEnabled(false);
    ui->exitPushButton->setEnabled(false);
    ui->runAllTestPushButton->setEnabled(false);
}

void MainTestsWidget::setEnabledRunButtonOfStatusWidgets(bool isEnabled)
{
    for (StatusCommonWidget *widget : m_statusWidgets.keys())
    {
        widget->setEnabledRunButton(isEnabled);
    }
}

void MainTestsWidget::showDetails(QString detailsText)
{
    m_detailsText->clear();
    m_detailsText->appendPlainText(detailsText);
}

void MainTestsWidget::showAllTest()
{

}

void MainTestsWidget::setWidgetStatus(ADTExecutable *task, StatusCommonWidget::WidgetStatus status, bool moveScroll)
{
    StatusCommonWidget *currentWidget = findWidgetByTask(task);

    if (!currentWidget)
    {
        return;
    }

    currentWidget->setWidgetStatus(status);

    if (moveScroll)
    {
        ui->summaryScrollArea->ensureWidgetVisible(currentWidget);
    }
}

std::vector<ADTExecutable *> MainTestsWidget::getTasks()
{
    std::vector<ADTExecutable *> tasks;

    for (StatusCommonWidget *widget : m_statusWidgets.keys())
    {
        tasks.push_back(widget->getExecutable());
    }

    return tasks;
}

void MainTestsWidget::on_runAllTestPushButton_clicked()
{
    m_controller->runTestsWidget(getTasks());
}

void MainTestsWidget::on_backPushButton_clicked()
{
    m_controller->backTestsWigdet();
}

void MainTestsWidget::onExitPushButtonClicked()
{
    m_controller->exitTestsWidget();
}

void MainTestsWidget::onRunButtonCurrentStatusWidgetClicked(StatusCommonWidget *widget)
{
    std::vector<ADTExecutable *> runningTests;

    runningTests.push_back(widget->getExecutable());

    m_controller->runTestsWidget(runningTests);
}

void MainTestsWidget::onDetailsButtonCurrentStatusWidgetClicked(StatusCommonWidget *widget)
{
    m_controller->detailsCurrentTest(widget->getExecutable());
}

void MainTestsWidget::onBackToSummaryButtonClicked()
{

}

void MainTestsWidget::updateStatusWidgets()
{
    if (!m_currentToolItem)
    {
        return;
    }

    m_statusWidgets.clear();

    if (m_currentToolItem->childCount() == 0)
    {
        return;
    }

    for (int i = m_currentToolItem->childCount() - 1; i >= 0; i--)
    {
        StatusCommonWidget *currentWidget = new StatusCommonWidget(m_currentToolItem->child(i));

        connect(currentWidget,
                &StatusCommonWidget::detailsButtonClicked,
                this,
                &MainTestsWidget::onDetailsButtonCurrentStatusWidgetClicked);

        connect(currentWidget,
                &StatusCommonWidget::runButtonCLicked,
                this,
                &MainTestsWidget::onRunButtonCurrentStatusWidgetClicked);

        m_statusWidgets[currentWidget] = 0;
    }
}

void MainTestsWidget::updateListOfStatusCommonWidgets()
{
    int i = 0;
    for (auto &commonStatusWidget : m_statusWidgets.keys())
    {
        m_summaryLayout->insertWidget(i, commonStatusWidget, Qt::AlignTop);
        i++;
    }

    m_summaryLayout->insertStretch(LAYOUT_STRETCH_INDEX, LAYOUT_STRETCH_FACTOR);
}

void MainTestsWidget::clearUi()
{
    delete ui->summaryScrollAreaWidgetContents;

    ui->summaryScrollAreaWidgetContents = new QWidget();

    ui->summaryScrollArea->setWidget(ui->summaryScrollAreaWidgetContents);

    m_summaryLayout = new QVBoxLayout();
    m_summaryLayout->setAlignment(Qt::AlignTop);

    ui->summaryScrollAreaWidgetContents->setLayout(m_summaryLayout);
}

StatusCommonWidget *MainTestsWidget::findWidgetByTask(ADTExecutable *task)
{
    for (StatusCommonWidget *currentWidget : m_statusWidgets.keys())
    {
        if (task == currentWidget->getExecutable())
        {
            return currentWidget;
        }
    }

    qWarning() << "ERROR: can't find status widget by task!";

    return nullptr;
}

void MainTestsWidget::on_exitPushButton_clicked()
{
    m_controller->exitTestsWidget();
}
