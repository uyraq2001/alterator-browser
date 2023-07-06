#include <QDebug>

#include "modulepushbutton.h"

ModulePushButton::ModulePushButton(QWidget *parent) :
    QPushButton(parent),
    iface(new QDBusInterface("", "")),
    mode(AlteratorModes::StandardMode)
{
    connect(this, &ModulePushButton::clicked, this, &ModulePushButton::onClicked);
}

ModulePushButton::~ModulePushButton()
{
    delete iface;
}

void ModulePushButton::setDBusInterface(const QString &service,
                      const QString &path,
                      const QString &interface,
                      const QDBusConnection &connection)
{
    delete iface;

    iface = new QDBusInterface(service, path, interface, connection);
}

QDBusInterface* ModulePushButton::getDBusInterface(){return iface;}

void ModulePushButton::setMode(AlteratorModes m){mode = m;}

AlteratorModes ModulePushButton::getMode(){return mode;}

void ModulePushButton::onClicked()
{
    if (!iface->isValid()){
        qWarning() << "ERROR: Interface '" + iface->interface() +
                      "' of object '" + iface->path() +
                      "' of service '" + iface->service()+
                      "' is invalid!";
        return;
    }
    if (mode == AlteratorModes::StandardMode){
        iface->call("RunSimple");
    }else if(mode == AlteratorModes::DevMode){
        iface->call("RunDev");
    }
}
