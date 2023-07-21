#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "modelbuilder.h"
#include "model/acobjectitem.h"

#include <QStandardItemModel>
#include <QTreeView>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QString>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>
#include <QDomElement>
#include <QDebug>
#include <QDomText>
#include <QStandardItem>
#include <QLayout>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(nullptr),
      controller(nullptr)
{
    ui->setupUi(this);

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->scrollArea->widget()->setLayout(categoryLayout);

    setWindowTitle(tr("altcenter"));

    centralWidget()->installEventFilter(this);
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
}

void MainWindow::setController(ACController *c){
    controller = c;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->scrollArea->viewport() && event->type() == QEvent::MouseButtonDblClick){

    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::setModel(ACModel *m)
{
    model = m;
    QLayout *categoryLayout = ui->scrollArea->widget()->layout();
    for (int i = 0; i < model->rowCount(); ++i){
        CategoryWidget *catWidget = new CategoryWidget();
        categoryLayout->addWidget(catWidget);
        catWidget->setItem(dynamic_cast<ACObjectItem *>(model->item(i)));
        connect(catWidget, &CategoryWidget::moduleClicked, this, &MainWindow::onModuleClicked);
        connect(this, &MainWindow::showMenu, catWidget, &CategoryWidget::showModuleMenu);
    }
}

void MainWindow::clearUi()
{
    QLayout *categoryLayout = ui->scrollArea->widget()->layout();
    while (categoryLayout->itemAt(0) != nullptr){
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
