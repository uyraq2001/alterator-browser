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
    QStringList params;
    params << "-l" << moduleItem->m_acObject.get()->m_icon;
    QProcess *proc = new QProcess(this);
    proc->start("alterator-standalone", params);


    window->showModuleMenu(moduleItem);
}

void ACController::onInterfaceClicked(ACLocalApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_exec, QStringList());
}
