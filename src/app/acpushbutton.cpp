#include "acpushbutton.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QProcess>

#include <model/aclocalapplication.h>
#include <model/acobject.h>

ACPushButton::ACPushButton(MainWindow *w, QWidget *parent)
    : data(nullptr)
    , window(w)
{
    setParent(parent);
    connect(this, &ACPushButton::clicked, window, [this]() { window->onModuleClicked(this); });
    connect(window, &MainWindow::showMenu, this, &ACPushButton::showMenu);
}

ACPushButton::~ACPushButton() {}

void ACPushButton::setItem(ACObjectItem *item)
{
    data = item;

    this->setText(item->getACObject()->m_displayName);
    this->setMinimumWidth(this->sizeHint().width());
    if (item->getACObject()->m_interfaces.empty())
    {
        //        setEnabled(false);
    }
}

ACObjectItem *ACPushButton::getItem()
{
    return data;
}

void ACPushButton::showMenu(ACObjectItem *item)
{
    if (item == this->data)
    {
        if (item->m_acObject.get()->m_applications.size() > 1)
        {
            QMenu *menu = new QMenu(this);
            for (auto i : item->m_acObject.get()->m_applications)
            {
                QAction *interfaceAction = new QAction("&" + i->m_implementedInterface, menu);
                menu->addAction(interfaceAction);
                connect(interfaceAction, &QAction::triggered, this, [i, this]() { window->onInterfaceClicked(i); });
            }
            this->setMenu(menu);
            QPushButton::showMenu();
        }
        else if (item->m_acObject.get()->m_applications.size() == 1)
        {
            auto app = item->m_acObject.get()->m_applications[0];
            window->onInterfaceClicked(app);
        }
    }
}
