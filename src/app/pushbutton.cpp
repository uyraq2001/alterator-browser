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

void PushButton::setItem(model::ObjectItem *newItem)
{
    try
    {
        this->item        = newItem;
        model::Object obj = std::get<ab::model::Object>(*newItem->getObject());

        this->setText(obj.m_displayName);
        this->setMinimumWidth(this->sizeHint().width());

        if (obj.m_applications.size() > 1)
        {
            auto menu = std::make_unique<QMenu>(this);
            for (const auto &app : obj.m_applications)
            {
                auto interfaceAction = std::make_unique<QAction>("&" + app->m_implementedInterface, menu.get());
                connect(interfaceAction.get(), &QAction::triggered, this, [app, this]() {
                    window->onInterfaceClicked(app);
                });
                menu->addAction(interfaceAction.release());
            }
            setMenu(menu.release());
        }
    }
    catch (const std::bad_variant_access &e)
    {
        qCritical() << "ERROR: the item is not of Object type";
    }
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
