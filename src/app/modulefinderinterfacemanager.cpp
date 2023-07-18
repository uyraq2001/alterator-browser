#include "modulefinderinterfacemanager.h"

#include <QDebug>
#include <sstream>

#include "alteratorparser.h"

const QString ALTERATORSERVICE = "ru.basealt.alterator";
const QString MODULEFINDERPATH = "/ru/basealt/alterator/module_finder";
const QString FINDERINTERFACE = "ru.basealt.alterator.finder";
const QString LISTMETHOD = "list";
const QString RETRIEVEALMETHOD = "file";

ModuleFinderInterfaceManager::ModuleFinderInterfaceManager(QObject *parent)
    : applications()
{
    setParent(parent);
    QDBusInterface *iface = new QDBusInterface(ALTERATORSERVICE,
                                              MODULEFINDERPATH,
                                              FINDERINTERFACE,
                                              QDBusConnection::systemBus());
    for (QString i: iface->call(LISTMETHOD).arguments()[0].toStringList()){
        i = i.left(i.size() - 1);
        QStringList reply = iface->call(RETRIEVEALMETHOD, i).arguments()[0].toStringList();
        QMap<QString, QVariant> parsed = parseDesktopFile(reply);
        applications.insert(i, parsed);
    }
}

ModuleFinderInterfaceManager::~ModuleFinderInterfaceManager(){}

QStringList ModuleFinderInterfaceManager::getApplications()
{
    return applications.keys();
}

QStringList ModuleFinderInterfaceManager::getAppInterfaces(QString app)
{
    QStringList res;
    QString iface = applications.value(app).value("Altcenter").toMap().value("interface").toString();
    res << iface;
    return res;
}

QMap<QString, QVariant> ModuleFinderInterfaceManager::parseDesktopFile(QStringList &data)
{
    QMap<QString, QVariant> res;
    QString concat;
    for (QString i: data){concat += i;}
    boost::property_tree::ptree pt;

    try{
        std::istringstream iStream(concat.toStdString());
        boost::property_tree::ini_parser::read_ini(iStream, pt);
    }
    catch (std::exception &e)
    {
        qWarning() << "ERROR: can't parse desktop file: " << data << "\n";
    }

    res = getNextLevelOfPtree(pt);

    return res;
}

QMap<QString, QVariant> ModuleFinderInterfaceManager::getNextLevelOfPtree(boost::property_tree::ptree pt)
{
    QMap<QString, QVariant> res;

    for (auto i: pt){
        res.insert(QString(i.first.c_str()), (i.second.empty() ?
                                                  QVariant(QString(i.second.get_value("").c_str())) :
                                                  QVariant(getNextLevelOfPtree(i.second))));
    }

    return res;
}
