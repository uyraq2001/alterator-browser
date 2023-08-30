#ifndef AB_PUSH_BUTTON_H
#define AB_PUSH_BUTTON_H

#include "model/objectitem.h"

#include <QPushButton>
#include <QWidget>

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

#endif // AB_PUSH_BUTTON_H
