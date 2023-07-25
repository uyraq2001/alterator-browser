#ifndef ACPUSHBUTTON_H
#define ACPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

#include "model/acobjectitem.h"

class ACPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ACPushButton(QWidget *parent = nullptr);
    ~ACPushButton();

    void setItem(ACObjectItem *item);
    ACObjectItem *getItem();

    void showMenu(ACObjectItem *item);

private:

    ACObjectItem *data;

    void onClicked(bool b);

    void interfaceClicked(ACLocalApplication *app);

    ACPushButton(const ACPushButton &) = delete;
    ACPushButton(ACPushButton &&)      = delete;
    ACPushButton &operator=(const ACPushButton &) = delete;
    ACPushButton &operator=(ACPushButton &&) = delete;

signals:

    void moduleClicked(ACPushButton *button);
};

#endif // ACPUSHBUTTON_H
