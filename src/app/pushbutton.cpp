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
    connect(this->window, &MainWindow::showMenu, this, &PushButton::showMenu);
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

void PushButton::showMenu()
{
    if (item->m_object->m_applications.size() == 1)
    {
        auto app = item->m_object->m_applications[0];
        window->onInterfaceClicked(app);
        return;
    }

    auto menu = std::make_unique<QMenu>(this);
    for (const auto &app : item->m_object->m_applications)
    {
        auto interfaceAction = std::make_unique<QAction>("&" + app->m_implementedInterface, menu.get());
        connect(interfaceAction.get(), &QAction::triggered, this, [app, this]() { window->onInterfaceClicked(app); });
        menu->addAction(interfaceAction.release());
    }
    this->setMenu(menu.release());
    QPushButton::showMenu();
}
} // namespace ab
