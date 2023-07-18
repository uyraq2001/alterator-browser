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
#include "modulefinderinterfacemanager.h"

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
            qWarning() << "Warning: object " +
                          objectPath +
                          " doesn't provide interface " +
                          APPLICATION_INTERFACE;
            continue;
        }

        QMap<QString, QString> info = readIniFromDBus(iface.call("info"));
        QStandardItem *moduleItem = new QStandardItem();
        moduleItem->setData(findTranslations("Name", info), Qt::DisplayRole);
        modules.insert(objectPath, moduleItem);

        QString categoryPath = info.value("Categories");
        if (!categories.contains(categoryPath)){
            QMap<QString, QString> category = readIniFromDBus(iface.call("category"));
            QStandardItem *categoryItem = new QStandardItem();
            categoryItem->setData(findTranslations("Name", category), Qt::DisplayRole);
            categoryItem->setData(findTranslations("Comment", category), UserRoles::DescriptionRole);
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

        ModuleFinderInterfaceManager *ifaceManager = new ModuleFinderInterfaceManager(this);
        QStringList appsDBus = ifaceManager->getApplications();
        for (QString j: appsDBus){
            for (QString k: ifaceManager->getAppInterfaces(j)){
                QStandardItem *ifaceItem = new QStandardItem();
                ifaceItem->setData(k, Qt::DisplayRole);
                moduleItem->appendRow(ifaceItem);
            }
        }
    }
    return true;
}

QVariantMap ModelBuilder::findTranslations(QString field, QMap<QString, QString> dump)
{
    QVariantMap res;

    if(!dump.contains(field)){
        qWarning() << "ERROR: '" + field + "' doesn't name a field!";
        return res;
    }else{
        res.insert("Default", dump.value(field));
    }

    QRegularExpression rx("^" + field + "\\[(.*)\\]$");

    for (auto i = dump.cbegin(); i != dump.cend(); ++i){
        QRegularExpressionMatch match = rx.match(i.key());
        if (match.hasMatch()) {
            QString locale = match.captured(1);
            res.insert(locale, i.value());
        }
    }
    return res;
}

QMap<QString, QString> ModelBuilder::readIniFromDBus(QDBusMessage mess){
    QMap<QString, QString> res;
    if (mess.type() == QDBusMessage::ErrorMessage){
        qWarning() << mess.errorMessage();
        return res;
    }
    QList<QString> resList = mess.arguments()[0].toStringList();
    for(QString j: resList){
        QStringList splited = j.split('=');
        if (splited.size() != 2){
            if (j != "[Desktop Entry]\n")
                qWarning() << "Warning: are you sure that '" + j + "' is  a valid .ini entry?";
            continue;
        }
        if (splited[1].contains("\n")){
            splited[1] = splited[1].left(splited[1].size() - 1);
        }
        res.insert(splited[0], splited[1]);
    }
    return res;
}
