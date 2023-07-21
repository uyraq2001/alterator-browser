#include "acpushbutton.h"

#include <QMenu>
#include <QAction>

ACPushButton::ACPushButton(QWidget *parent)
    : data(nullptr)
{
    setParent(parent);
}

ACPushButton::~ACPushButton(){}

void ACPushButton::setItem(ACObjectItem *item)
{
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
