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

MainWindow::MainWindow(QStandardItemModel *m, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(m)
{
    ui->setupUi(this);

    setWindowTitle(tr("altcenter"));

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QWidget *scrollWidget = new QWidget();
    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->widget()->setLayout(categoryLayout);

    for (int i = 0; i < m->rowCount(); ++i){
//        QString text = m->index(i, 0).data(Qt::DisplayRole).toString();
//        QString text = dynamic_cast<ACObjectItem *>(m->itemFromIndex(m->index(i, 0)))->m_acObject.get()->m_displayCategory;
        CategoryWidget *catWidget = new CategoryWidget
                (dynamic_cast<ACObjectItem *>(m->itemFromIndex(m->index(i, 0))));
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
    QWidget *scrollWidget = ui->scrollArea->widget();
    scrollWidget->setMinimumWidth(scrollWidget->layout()->minimumSize().width());
    ui->scrollArea->setMinimumWidth(scrollWidget->minimumWidth());
    this->setMinimumWidth(ui->scrollArea->minimumWidth());
}
