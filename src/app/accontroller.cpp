#include "accontroller.h"

#include <QAction>
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>

ACController::ACController(MainWindow *w, ACModel *m, QObject *parent)
    : QObject{parent}
    , window(w)
    , model(m)
{
    if (model)
    {
        w->setModel(m);
    }
}

ACController::~ACController() {}

void ACController::moduleClicked(ACObjectItem *moduleItem)
{
    if (moduleItem->m_acObject->m_isLegacy){
        QProcess *proc = new QProcess(this);
        proc->start("alterator-standalone", QStringList() << "-l" << moduleItem->m_acObject.get()->m_icon);
    }else{
        window->showModuleMenu(moduleItem);
    }
}

void ACController::onInterfaceClicked(ACLocalApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_exec, QStringList());
}
