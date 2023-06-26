#include "moduleswidget.h"
#include "ui_moduleswidget.h"

#include <QDebug>
#include <QMouseEvent>

ModulesWidget::ModulesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModulesWidget)
{
    ui->setupUi(this);

    ui->toolsListView->viewport()->installEventFilter(this);
}

ModulesWidget::~ModulesWidget()
{
    delete ui;
}

void ModulesWidget::enableButtons()
{
    ui->runAllPushButton->setEnabled(true);
    ui->browseCheckPushButton->setEnabled(true);
}

void ModulesWidget::disableButtons()
{
    ui->runAllPushButton->setEnabled(false);
    ui->browseCheckPushButton->setEnabled(false);
}

void ModulesWidget::enableExitButton()
{
    ui->exitPushButton->setEnabled(true);
}

void ModulesWidget::disableExitButton()
{
    ui->exitPushButton->setEnabled(false);
}

void ModulesWidget::setModel(QAbstractItemModel *model)
{
    ui->toolsListView->setModel(model);

    connect(ui->toolsListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &ModulesWidget::onSelectionChanged);
}

void ModulesWidget::setDescription(QString description)
{
    ui->descriptionTextEdit->setText(description);
}

bool ModulesWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->toolsListView->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QModelIndex index       = ui->toolsListView->indexAt(mouseEvent->pos());

        if (index.isValid())
        {
            TreeItem *selectedItem = static_cast<TreeItem *>(index.internalPointer());
            m_controller->changeSelectedTool(selectedItem);
            on_browseCheckPushButton_clicked();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ModulesWidget::on_runAllPushButton_clicked()
{
    m_controller->runAllToolsWidget();
}

void ModulesWidget::on_browseCheckPushButton_clicked()
{
    m_controller->chooseToolsWidget();
}

void ModulesWidget::on_exitPushButton_clicked()
{
    m_controller->exitToolsWidget();
}

void ModulesWidget::onSelectionChanged(const QItemSelection &newSelection, const QItemSelection &previousSelection)
{
    if (newSelection.isEmpty())
    {
        return;
    }

    QModelIndex currentIndex = newSelection.indexes().at(0);
    TreeItem *currentItem    = static_cast<TreeItem *>(currentIndex.internalPointer());

    if (!currentItem)
    {
        qWarning() << "ERROR: Can't get item from selection model!";
    }

    m_controller->changeSelectedTool(currentItem);
}
