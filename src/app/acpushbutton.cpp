#include "acpushbutton.h"

#include <QMenu>
#include <QAction>

ACPushButton::ACPushButton(QWidget *parent)
    : data(nullptr)
{
    setParent(parent);
    connect(this, &ACPushButton::clicked, this, &ACPushButton::onClicked);
}

ACPushButton::~ACPushButton(){}

void ACPushButton::setItem(ACObjectItem *item)
{
    data = item;

    this->setText(item->getACObject()->m_displayName);
    this->setMinimumWidth(this->sizeHint().width());
//    connect(moduleButton, &QPushButton::clicked, this, &CategoryWidget::onClicked);

    QMenu *moduleMenu = new QMenu(this);
    for (int j = 0; j < item->model()->rowCount(item->index()); ++j){
        QStandardItem *ifaceItem = item->child(j);
        QAction *ifaceAction = new QAction(
                    "&" + ifaceItem->data(Qt::DisplayRole).toString(),
                    moduleMenu);

        moduleMenu->addAction(ifaceAction);
    }
//        moduleButton->setMenu(moduleMenu);
//        moduleMenu->installEventFilter(this);

}

ACObjectItem *ACPushButton::getItem()
{
    return data;
}

void ACPushButton::onClicked(bool c)
{
    emit moduleClicked(this);
}

void ACPushButton::showMenu(ACObjectItem *item)
{
    if (item == this->data){
        QMenu *menu = new QMenu(this);
        for (auto i: item->m_acObject.get()->m_interfaces){
            QAction *interfaceAction = new QAction("&" + i, menu);
            menu->addAction(interfaceAction);
        }
        this->setMenu(menu);
    }
}
