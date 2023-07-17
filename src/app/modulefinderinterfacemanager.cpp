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
    : m_appInterfaces()
{
    setParent(parent);
    QDBusInterface *iface = new QDBusInterface(ALTERATORSERVICE,
                                              MODULEFINDERPATH,
                                              FINDERINTERFACE,
                                              QDBusConnection::systemBus());
    for (QString i: iface->call(LISTMETHOD).arguments()[0].toStringList()){
        i = i.left(i.size() - 1);
        QStringList reply = iface->call(RETRIEVEALMETHOD, i).arguments()[0].toStringList();
        QMultiMap<QString, QString> parsed = parseDesktopFile(reply);
        m_appInterfaces = m_appInterfaces.unite(parsed);
    }
}

ModuleFinderInterfaceManager::~ModuleFinderInterfaceManager(){}

QStringList ModuleFinderInterfaceManager::getApplications()
{
    return m_appInterfaces.keys();
}

QStringList ModuleFinderInterfaceManager::getAppInterfaces(QString app)
{
    return m_appInterfaces.values(app);
}

QMultiMap<QString, QString> ModuleFinderInterfaceManager::parseDesktopFile(QStringList &data)
{
    QMultiMap<QString, QString> res;
    QString concat;
    for (QString i: data){concat += i;}

    QString exec;
    QString iface;

    try
    {
        std::istringstream iStream(concat.toStdString());
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(iStream, pt);

        for (auto &section : pt)
        {
            QString kcurrentSections = section.first.c_str();

            if(kcurrentSections == "Desktop Entry") {
                for (auto &key : section.second)
                {
                    QString keyName          = QString(key.first.c_str());
                    QString keyValue         = key.second.get_value("").c_str();
                    if(keyName == "Exec") {
                        exec = keyValue;
                    }
                    else {
                        continue;
                    }
                }
            } else if (kcurrentSections == "Altcenter") {
                for (auto &key : section.second)
                {
                    QString keyName          = QString(key.first.c_str());
                    QString keyValue         = key.second.get_value("").c_str();
                    if(keyName == "interface") {
                        iface = keyValue;
                    }
                    else {
                        continue;
                    }
                }

            } else {
                continue;
            }
        }

        if(!exec.isEmpty() && !iface.isEmpty()) {
            res.insert(iface, exec);
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "ERROR: can't parse desktop file: " << data << "\n";
    }
    return res;
}

