#include "mainwindow.h"
#include "categorywidget.h"
#include "controller.h"
#include "mainwindowsettings.h"
#include "model/objectitem.h"
#include "ui_mainwindow.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomText>
#include <QLayout>
#include <QMouseEvent>
#include <QShortcut>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

namespace ab
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(new MainWindowSettings(this, ui))
{
    ui->setupUi(this);
    settings.get()->restoreSettings();

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->scrollArea->widget()->setLayout(categoryLayout);

    setWindowTitle(tr("Alterator Browser"));
    setWindowIcon(QIcon(":/logo.png"));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QWidget *scrollWidget = this->ui->scrollArea->widget();
    scrollWidget->setMinimumWidth(scrollWidget->layout()->minimumSize().width());
    this->ui->scrollArea->setMinimumWidth(scrollWidget->minimumWidth());
    this->setMinimumWidth(ui->scrollArea->minimumWidth());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings.get()->saveSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::setController(Controller *newContoller)
{
    this->controller = newContoller;
}

void MainWindow::setModel(model::Model *newModel)
{
    this->model             = newModel;
    QLayout *categoryLayout = this->ui->scrollArea->widget()->layout();
    for (int i = 0; i < this->model->rowCount(); ++i)
    {
        CategoryWidget *categoryWidget = new CategoryWidget(this);
        categoryLayout->addWidget(categoryWidget);
        categoryWidget->setItem(dynamic_cast<model::ObjectItem *>(this->model->item(i)));
    }
}

void MainWindow::clearUi()
{
    QLayout *categoryLayout = this->ui->scrollArea->widget()->layout();
    while (categoryLayout->itemAt(0) != nullptr)
    {
        QWidget *categoryWidget = categoryLayout->itemAt(0)->widget();
        categoryLayout->removeWidget(categoryWidget);
        delete categoryWidget;
    }
}

void MainWindow::onModuleClicked(PushButton *button)
{
    this->controller->moduleClicked(button->getItem());
}

void MainWindow::showModuleMenu(model::ObjectItem *item)
{
    emit showMenu(item);
}

void MainWindow::onInterfaceClicked(model::LocalApplication *app)
{
    this->controller->onInterfaceClicked(app);
}
} // namespace ab
