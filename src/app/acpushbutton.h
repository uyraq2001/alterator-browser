#ifndef ACPUSHBUTTON_H
#define ACPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>

#include "mainwindow.h"
#include "model/acobjectitem.h"

class MainWindow;

class ACPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ACPushButton(MainWindow *w, QWidget *parent = nullptr);
    ~ACPushButton();

    void setItem(ACObjectItem *item);
    ACObjectItem *getItem();

    void showMenu(ACObjectItem *item);

private:
    ACObjectItem *data;
    MainWindow *window;

    void onClicked(bool b);

    ACPushButton(const ACPushButton &) = delete;
    ACPushButton(ACPushButton &&)      = delete;
    ACPushButton &operator=(const ACPushButton &) = delete;
    ACPushButton &operator=(ACPushButton &&) = delete;

signals:

    void moduleClicked(ACPushButton *button);
};

#endif // ACPUSHBUTTON_H
