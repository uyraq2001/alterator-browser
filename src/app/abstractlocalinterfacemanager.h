#ifndef ABSTRACTLOCALINTERFACEMANAGER_H
#define ABSTRACTLOCALINTERFACEMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDBusInterface>
#include <QVariant>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class AbstractLocalInterfaceManager : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractLocalInterfaceManager();

    virtual QStringList getApplications() = 0;

    virtual QStringList getAppInterfaces(QString app) = 0;
};

#endif // ABSTRACTLOCALINTERFACEMANAGER_H
