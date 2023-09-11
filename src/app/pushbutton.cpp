#include "pushbutton.h"
#include "mainwindow.h"
#include "model/localapplication.h"
#include "model/object.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QProcess>

namespace ab
{
PushButton::PushButton(MainWindow *w, QWidget *parent)
    : item(nullptr)
    , window(w)
{
    setParent(parent);
    connect(this, &PushButton::clicked, this->window, [this]() { this->window->onModuleClicked(this); });
}

void PushButton::setItem(model::ObjectItem *newItem)
{
    this->item = newItem;

    this->setText(item->getObject()->m_displayName);
    this->setMinimumWidth(this->sizeHint().width());
}

model::ObjectItem *PushButton::getItem()
{
    return item;
}

void PushButton::showMenu(std::unique_ptr<QMenu> menu)
{
    this->setMenu(menu.release());
    QPushButton::showMenu();
}
} // namespace ab
