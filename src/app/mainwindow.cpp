#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "modelbuilder.h"
#include "browserdelegate.h"

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

MainWindow::MainWindow(QStandardItemModel *m, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(m)
{
    ui->setupUi(this);

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QWidget *scrollWidget = new QWidget();
    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->widget()->setLayout(categoryLayout);
//    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setMinimumWidth(scrollWidget->minimumSize().width());
    centralWidget()->setMinimumWidth(scrollWidget->sizeHint().width());

    for (int i = 0; i < m->rowCount(); ++i){
        QString text = m->index(i, 0).data(Qt::DisplayRole).toString();
//        if (text == "Users" || text == "System"|| text == "Network"|| text == "Firewall") continue;
        CategoryWidget *catWidget = new CategoryWidget(m, i);
        categoryLayout->addWidget(catWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}
