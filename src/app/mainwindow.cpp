#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model(new QStandardItemModel(this))//,
//      modulesView(new QTreeView(this))
{
    ui->setupUi(this);
    QDBusInterface *introspect = new QDBusInterface("ru.basealt.alterator",
                                                  "/ru/basealt/alterator",
                                                  "org.freedesktop.DBus.Introspectable",
                                                  QDBusConnection::systemBus());
    QString introspection = introspect->call("Introspect").arguments()[0].toStringList()[0];
    QDomDocument xml;
    xml.setContent(introspection);
    QList<QString> nodenames;
    QDomNodeList nodes = xml.elementsByTagName("node");
    for(int i = 0; i < nodes.length(); i++){
        QDomElement e = nodes.item(i).toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            nodenames.append(e.attribute("name"));
        }
    }
    for(QString i : nodenames){
        model->appendRow(new QStandardItem(i));
    }
    ui->modulesView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
