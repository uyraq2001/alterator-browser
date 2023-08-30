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
    : data(nullptr)
    , window(w)
{
    setParent(parent);
    connect(this, &PushButton::clicked, window, [this]() { window->onModuleClicked(this); });
    connect(window, &MainWindow::showMenu, this, &PushButton::showMenu);
}

PushButton::~PushButton() {}

void PushButton::setItem(model::ObjectItem *item)
{
    data = item;

    this->setText(item->getACObject()->m_displayName);
    this->setMinimumWidth(this->sizeHint().width());
}

model::ObjectItem *PushButton::getItem()
{
    return data;
}

void PushButton::showMenu(model::ObjectItem *item)
{
    if (item == this->data)
    {
        if (item->m_acObject->m_applications.size() > 1)
        {
            QMenu *menu = new QMenu(this);
            for (auto i : item->m_acObject->m_applications)
            {
                QAction *interfaceAction = new QAction("&" + i->m_implementedInterface, menu);
                menu->addAction(interfaceAction);
                connect(interfaceAction, &QAction::triggered, this, [i, this]() { window->onInterfaceClicked(i); });
            }
            this->setMenu(menu);
            QPushButton::showMenu();
        }
        else if (item->m_acObject->m_applications.size() == 1)
        {
            auto app = item->m_acObject->m_applications[0];
            window->onInterfaceClicked(app);
        }
    }
}
} // namespace ab
