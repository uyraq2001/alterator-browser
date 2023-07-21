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

private:

    ACObjectItem *data;

    ACPushButton(const ACPushButton &) = delete;
    ACPushButton(ACPushButton &&)      = delete;
    ACPushButton &operator=(const ACPushButton &) = delete;
    ACPushButton &operator=(ACPushButton &&) = delete;

signals:

};

#endif // ACPUSHBUTTON_H
