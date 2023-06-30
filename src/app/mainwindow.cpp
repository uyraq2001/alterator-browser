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

MainWindow::MainWindow(QStandardItemModel *m, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(m)
{
    ui->setupUi(this);

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    for (int i = 0; i < m->rowCount(); ++i){
        QString text = m->index(i, 0).data(Qt::DisplayRole).toString();
        categoryLayout->addWidget(new CategoryWidget(m, i));
    }

    ui->scrollArea->widget()->setLayout(categoryLayout);
//    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//    ui->centralwidget->setMinimumSize(ui->centralwidget->sizeHint());
//    setMinimumSize(sizeHint());

//    qWarning() << size();
//    qWarning() << minimumSize();
//    qWarning() << centralWidget()->sizeHint();
    //    categoryLayout->addWidget(new CategoryWidget(model, -1));
}

MainWindow::~MainWindow()
{
    delete ui;
}
