#ifndef AB_PUSH_BUTTON_H
#define AB_PUSH_BUTTON_H

#include "model/objectitem.h"

#include <QPushButton>
#include <QWidget>

namespace ab
{
class MainWindow;

class PushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PushButton(MainWindow *w, QWidget *parent = nullptr);
    ~PushButton();

    void setItem(model::ObjectItem *item);
    model::ObjectItem *getItem();

    void showMenu(model::ObjectItem *item);

private:
    model::ObjectItem *data;
    MainWindow *window;

    void onClicked(bool b);

    PushButton(const PushButton &) = delete;
    PushButton(PushButton &&)      = delete;
    PushButton &operator=(const PushButton &) = delete;
    PushButton &operator=(PushButton &&) = delete;

signals:

    void moduleClicked(PushButton *button);
};
} // namespace ab

#endif // AB_PUSH_BUTTON_H
