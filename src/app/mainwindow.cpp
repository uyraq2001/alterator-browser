#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "model/acobjectitem.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(nullptr)
    , controller(nullptr)
    , settings(new MainWindowSettings(this, ui))
{
    ui->setupUi(this);
    settings.get()->restoreSettings();

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->scrollArea->widget()->setLayout(categoryLayout);

    setWindowTitle(tr("altcenter"));
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
    QWidget *scrollWidget = ui->scrollArea->widget();
    scrollWidget->setMinimumWidth(scrollWidget->layout()->minimumSize().width());
    ui->scrollArea->setMinimumWidth(scrollWidget->minimumWidth());
    this->setMinimumWidth(ui->scrollArea->minimumWidth());
    settings.get()->saveSettings();
}

void MainWindow::setController(ACController *c)
{
    controller = c;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->scrollArea->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {}
    return QWidget::eventFilter(watched, event);
}

void MainWindow::setModel(ACModel *m)
{
    model                   = m;
    QLayout *categoryLayout = ui->scrollArea->widget()->layout();
    for (int i = 0; i < model->rowCount(); ++i)
    {
        CategoryWidget *catWidget = new CategoryWidget(this);
        categoryLayout->addWidget(catWidget);
        catWidget->setItem(dynamic_cast<ACObjectItem *>(model->item(i)));
    }
}

void MainWindow::clearUi()
{
    QLayout *categoryLayout = ui->scrollArea->widget()->layout();
    while (categoryLayout->itemAt(0) != nullptr)
    {
        QWidget *categoryWidget = categoryLayout->itemAt(0)->widget();
        categoryLayout->removeWidget(categoryWidget);
        delete categoryWidget;
    }
}

void MainWindow::onModuleClicked(ACPushButton *button)
{
    controller->moduleClicked(button->getItem());
}

void MainWindow::showModuleMenu(ACObjectItem *item)
{
    emit showMenu(item);
}

void MainWindow::onInterfaceClicked(ACLocalApplication *app)
{
    controller->onInterfaceClicked(app);
}
