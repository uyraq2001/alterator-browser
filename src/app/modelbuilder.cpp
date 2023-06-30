#include "modelbuilder.h"
#include "../core/adtdesktopfileparser.h"

#include <QString>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDomDocument>
#include <QDebug>
#include <QDBusReply>

const QString ALTERATOR_SERVICE = "ru.basealt.alterator";
const QString ROOT_OBJECT = "/ru/basealt/alterator";
const QString INTROSPECTABLE_INTERFACE = "org.freedesktop.DBus.Introspectable";
const QString APPLICATION_INTERFACE = "ru.basealt.alterator.application";

ModelBuilder::ModelBuilder(
        QStandardItemModel *m, QObject *parent)
    : QObject{parent}
    , model(m)
{}

bool ModelBuilder::build(){
    QDBusInterface introspect(ALTERATOR_SERVICE,
                              ROOT_OBJECT,
                              INTROSPECTABLE_INTERFACE,
                              QDBusConnection::systemBus());
    if (!introspect.isValid())
    {
        qWarning() << "ERROR: interface is invalid!";
        return false;
    }
    QString introspection = introspect.call("Introspect").arguments()[0].toStringList()[0];

    QDomDocument xml;

    xml.setContent(introspection);

    QStringList nodenames;

    QDomNodeList nodes = xml.elementsByTagName("node");
    for(int i = 0; i < nodes.length(); i++){
        QDomElement e = nodes.item(i).toElement();
        if(!e.isNull()) {
            nodenames.append(e.attribute("name"));
        }
    }
    QMap<QString, QStandardItem *> modules;

    QMap<QString, QStandardItem *> categories;

    for(QString i : nodenames){
        QDBusInterface iface(ALTERATOR_SERVICE,
                             ROOT_OBJECT + (i.isEmpty() ? "" : "/") + i,
                             APPLICATION_INTERFACE,
                             QDBusConnection::systemBus());
        if (!iface.isValid()){
            qWarning() << "Warning: object " + ROOT_OBJECT + "/" + i + " doesn't provide interface " + APPLICATION_INTERFACE;
            continue;
        }

        QDBusMessage infoMess = iface.call("info");
        if (infoMess.type() == QDBusMessage::ErrorMessage){
            qWarning() << infoMess.errorMessage();
            continue;
        }
        QMap<QString, QString> info;
        QList<QString> infoList = infoMess.arguments()[0].toStringList();
        for(QString j: infoList){
            QStringList splited = j.split('=');
            if (splited.size() != 2){
                qWarning() << "Warning: are you sure that '" + j + "' is  a valid .desktop entry?";
                continue;
            }
            if (splited[1].contains("\n")){
                splited[1] = splited[1].left(splited[1].size() - 1);
            }
            info.insert(splited[0], splited[1]);
        }
        QStandardItem *moduleItem = new QStandardItem();
        moduleItem->setData(info.value("Name"), Qt::DisplayRole);
        modules.insert(i, moduleItem);

        QString categoryPath = info.value("Categories");
        if (!categories.contains(categoryPath)){
            QDBusMessage categoryMess = iface.call("category");
            if (categoryMess.type() == QDBusMessage::ErrorMessage){
                qWarning() << categoryMess.errorMessage();
                continue;
            }
            QMap<QString, QString> category;
            QList<QString> categoryList = categoryMess.arguments()[0].toStringList();
            for(QString j: categoryList){
                QStringList splited = j.split('=');
                if (splited.size() != 2){
                    qWarning() << "Warning: are you sure that '" + j + "' is  a valid .directory entry?";
                    continue;
                }
                if (splited[1].contains("\n")){
                    splited[1] = splited[1].left(splited[1].size() - 1);
                }
                category.insert(splited[0], splited[1]);
            }

            QStandardItem *categoryItem = new QStandardItem();
            categoryItem->setData(category.value("Name"), Qt::DisplayRole);
            QPixmap icon = QPixmap(
                        "/usr/share/aterator/desin/images"
                        + category.value("Icon")+ ".png");
            categoryItem->setData(icon, Qt::DecorationRole);
            categories.insert(categoryPath, categoryItem);
            model->appendRow(categoryItem);
            categoryItem->appendRow(moduleItem);
        }else{
            categories.value(categoryPath)->appendRow(moduleItem);
        }
    }
    return true;
}
