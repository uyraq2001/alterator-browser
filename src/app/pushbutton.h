#ifndef AB_PUSH_BUTTON_H
#define AB_PUSH_BUTTON_H

#include "objectitem.h"

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
    ~PushButton() override = default;

    void setItem(model::ObjectItem *item);
    model::ObjectItem *getItem();

    void showMenu(std::unique_ptr<QMenu> menu);

    PushButton(const PushButton &) = delete;
    PushButton(PushButton &&)      = delete;
    PushButton &operator=(const PushButton &) = delete;
    PushButton &operator=(PushButton &&) = delete;

private:
    model::ObjectItem *item = nullptr;
    MainWindow *window      = nullptr;

    void onClicked(bool b);

protected:
    bool event(QEvent *event) override;

signals:
    void moduleClicked(PushButton *button);
};
} // namespace ab

#endif // AB_PUSH_BUTTON_H
