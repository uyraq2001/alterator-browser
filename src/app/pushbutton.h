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
    ~PushButton() = default;

    void setItem(model::ObjectItem *item);
    model::ObjectItem *getItem();

    void showMenu();

private:
    model::ObjectItem *item = nullptr;
    MainWindow *window = nullptr;

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
