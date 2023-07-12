#ifndef MODULEPUSHBUTTON_H
#define MODULEPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QDBusInterface>
#include <QStandardItemModel>

#include "../core/enums.h"

class ModulePushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ModulePushButton(QStandardItemModel *m,
                              QStandardItem *item,
                              QWidget *parent = nullptr);
    ~ModulePushButton();

    void setDBusInterface(const QString &service,
                          const QString &path,
                          const QString &interface = QString(),
                          const QDBusConnection &connection =
            QDBusConnection::sessionBus());
    QDBusInterface *getDBusInterface();

    void setMode(AlteratorModes m);
    AlteratorModes getMode();

    void setText(const QString &text);

private:
    ModulePushButton(const ModulePushButton &) = delete;
    ModulePushButton(ModulePushButton &&)      = delete;
    ModulePushButton &operator=(const ModulePushButton &) = delete;
    ModulePushButton &operator=(ModulePushButton &&) = delete;

    QDBusInterface *iface;
    AlteratorModes mode;

    void onClicked();
};

#endif // MODULEPUSHBUTTON_H
