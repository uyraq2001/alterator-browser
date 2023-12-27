#include "pushbutton.h"
#include "mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QProcess>

namespace ab
{
PushButton::PushButton(MainWindow *w, QWidget *parent)
    : window(w)
{
    setParent(parent);
    connect(this, &PushButton::clicked, this->window, [this]() { this->window->onModuleClicked(this); });
}

void PushButton::setObject(ao_builder::Object *obj)
{
    this->object = obj;
    this->setText(obj->m_displayName);
    this->setStyleSheet("padding: 3px 7px;");

    QFont font = this->font();
    font.setPointSize(11);
    this->setFont(font);
}

ao_builder::Object *PushButton::getObject()
{
    return object;
}

bool PushButton::event(QEvent *event)
{
    if (event->type() == QEvent::HoverEnter)
    {
        setFlat(false);
    }

    if (event->type() == QEvent::HoverLeave)
    {
        setFlat(true);
    }

    return QPushButton::event(event);
}
} // namespace ab
