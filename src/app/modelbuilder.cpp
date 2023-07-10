#include "modelbuilder.h"
#include "../core/adtdesktopfileparser.h"

#include <QString>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDomDocument>
#include <QDebug>
#include <QDBusReply>
#include <QDBusArgument>

#include "../core/enums.h"

const QString ALTERATOR_SERVICE = "ru.basealt.alterator";
const QString ROOT_OBJECT = "/ru/basealt/alterator";
const QString INTROSPECTABLE_INTERFACE = "org.freedesktop.DBus.Introspectable";
const QString APPLICATION_INTERFACE = "ru.basealt.alterator.application";
const QString MANAGER_INTERFACE = "ru.basealt.alterator.manager";

ModelBuilder::ModelBuilder(QObject *parent)
    : QObject{parent}
{}

bool ModelBuilder::build(QStandardItemModel *model){
    QDBusInterface manager(ALTERATOR_SERVICE,
                              ROOT_OBJECT,
                              MANAGER_INTERFACE,
                              QDBusConnection::systemBus());
    if (!manager.isValid())
    {
        qWarning() << "ERROR: interface is invalid!";
        return false;
    }
    QDBusArgument dbusReply = manager.
            call("get_objects", "ru.basealt.alterator.application").
            arguments()[0].value<QDBusArgument>();
    QStringList displayableObjects;
    dbusReply.beginArray();
    while (!dbusReply.atEnd()) {
            QString element;
            dbusReply >> element;
            displayableObjects.append(element);
    }

    QMap<QString, QStandardItem *> modules;
    QMap<QString, QStandardItem *> categories;
    for(QVariant i : displayableObjects){
        QString objectPath = i.toString();
        QDBusInterface iface(ALTERATOR_SERVICE,
                             objectPath,
                             APPLICATION_INTERFACE,
                             QDBusConnection::systemBus());
        if (!iface.isValid()){
            qWarning() << "Warning: object " + objectPath + " doesn't provide interface " + APPLICATION_INTERFACE;
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
                if (j != "[Desktop Entry]\n")
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
        QMap<QString, QVariant> actionIfaceData = QMap<QString, QVariant>();
        actionIfaceData.insert("service", iface.service());
        actionIfaceData.insert("path", iface.path());
        actionIfaceData.insert("interface", iface.interface());
        actionIfaceData.insert("bus", "sessionBus");
        moduleItem->setData(actionIfaceData, UserRoles::ActionRole);
        modules.insert(objectPath, moduleItem);

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
                    if (j != "[Desktop Entry]\n")
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
            categoryItem->setData(category.value("Comment"), UserRoles::DescriptionRole);
            QPixmap icon = QPixmap(
                        "/usr/share/alterator/design/images/"
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
