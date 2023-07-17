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
    QMultiMap<QString, QString> parseDesktopFile(QStringList &data);

private:
    QMultiMap<QString, QString> m_appInterfaces;
};

#endif // MODULEFINDERINTERFACEMANAGER_H
