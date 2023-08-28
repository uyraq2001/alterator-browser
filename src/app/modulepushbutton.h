#ifndef MODULEPUSHBUTTON_H
#define MODULEPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QDBusInterface>
#include <QStandardItemModel>

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

    void setText(const QString &text);

private:
    ModulePushButton(const ModulePushButton &) = delete;
    ModulePushButton(ModulePushButton &&)      = delete;
    ModulePushButton &operator=(const ModulePushButton &) = delete;
    ModulePushButton &operator=(ModulePushButton &&) = delete;

    QDBusInterface *iface;

    void onClicked();
};

#endif // MODULEPUSHBUTTON_H
