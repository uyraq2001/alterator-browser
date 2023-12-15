#ifndef AB_PUSH_BUTTON_H
#define AB_PUSH_BUTTON_H

#include "../aobuilder/objects/legacyobject.h"

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

    void setObject(ao_builder::LegacyObject obj);
    ao_builder::LegacyObject getObject();

    PushButton(const PushButton &) = delete;
    PushButton(PushButton &&)      = delete;
    PushButton &operator=(const PushButton &) = delete;
    PushButton &operator=(PushButton &&) = delete;

private:
    ao_builder::LegacyObject object = {};
    MainWindow *window              = nullptr;

    void onClicked(bool b);

protected:
    bool event(QEvent *event) override;

signals:
    void moduleClicked(PushButton *button);
};
} // namespace ab

#endif // AB_PUSH_BUTTON_H
