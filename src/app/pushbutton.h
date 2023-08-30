#ifndef ACPUSHBUTTON_H
#define ACPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>

#include "mainwindow.h"
#include "model/objectitem.h"

class MainWindow;

class PushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PushButton(MainWindow *w, QWidget *parent = nullptr);
    ~PushButton();

    void setItem(ObjectItem *item);
    ObjectItem *getItem();

    void showMenu(ObjectItem *item);

private:
    ObjectItem *data;
    MainWindow *window;

    void onClicked(bool b);

    PushButton(const PushButton &) = delete;
    PushButton(PushButton &&)      = delete;
    PushButton &operator=(const PushButton &) = delete;
    PushButton &operator=(PushButton &&) = delete;

signals:

    void moduleClicked(PushButton *button);
};

#endif // ACPUSHBUTTON_H
