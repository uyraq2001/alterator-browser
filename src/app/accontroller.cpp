#include "accontroller.h"

#include <QAction>
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>

#include "model/acobject.h"
#include "model/acapplication.h"

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
    if (moduleItem->m_acEntity->toObject()->m_isLegacy){
        QProcess *proc = new QProcess(this);
        proc->start("alterator-standalone", QStringList() << "-l" << moduleItem->m_acEntity->toObject()->m_icon);
    }else{
        window->showModuleMenu(moduleItem);
    }
}

void ACController::onInterfaceClicked(ACApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_exec, QStringList());
}
