#ifndef MODULEFINDERINTERFACEMANAGER_H
#define MODULEFINDERINTERFACEMANAGER_H

#include "abstractlocalinterfacemanager.h"

class ModuleFinderInterfaceManager : public AbstractLocalInterfaceManager
{
    Q_OBJECT
public:
    ModuleFinderInterfaceManager(QObject *p = nullptr);
    ~ModuleFinderInterfaceManager();

    QStringList getApplications() override;
    QStringList getAppInterfaces(QString app) override;

private:
    QMap<QString, QVariant> parseDesktopFile(QStringList &data);
    QMap<QString, QVariant> getNextLevelOfPtree(boost::property_tree::ptree pt);

    QMap<QString, QMap<QString, QVariant>> applications;
};

#endif // MODULEFINDERINTERFACEMANAGER_H
