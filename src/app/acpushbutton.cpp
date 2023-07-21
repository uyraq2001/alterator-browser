#include "acpushbutton.h"

#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QDebug>

#include <model/acobject.h>
#include <model/aclocalapplication.h>

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
    if (item->getACObject()->m_interfaces.empty()){
        setEnabled(false);
        setStyleSheet("QPushButton{color: gray;}");
    }
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
        for (auto i: item->m_acObject.get()->m_applications){
            QAction *interfaceAction = new QAction("&" + i->m_exec, menu);
            connect(interfaceAction, &QAction::triggered,
                    this, [i, this]()
            {this->interfaceClicked(i);});
            menu->addAction(interfaceAction);
        }
        this->setMenu(menu);
    }
}

void ACPushButton::interfaceClicked(ACLocalApplication *app)
{
    qWarning() << "interface clicked";
    QProcess *proc = new QProcess(this);
    proc->start(app->m_exec, QStringList());
}
