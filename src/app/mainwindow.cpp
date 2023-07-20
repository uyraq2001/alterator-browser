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

MainWindow::MainWindow(QStandardItemModel *m, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(m),
      controller(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(tr("altcenter"));

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QWidget *scrollWidget = new QWidget();
    ui->scrollArea->setWidget(scrollWidget);
    ui->scrollArea->widget()->setLayout(categoryLayout);

    for (int i = 0; i < m->rowCount(); ++i){
        CategoryWidget *catWidget = new CategoryWidget
                (dynamic_cast<ACObjectItem *>(m->itemFromIndex(m->index(i, 0))));
        categoryLayout->addWidget(catWidget);
    }
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
