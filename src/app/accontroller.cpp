#include "accontroller.h"

#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QScrollArea>
#include <QAction>
#include <QMenu>
#include <QProcess>

ACController::ACController(MainWindow *w, ACModel *m, QObject *parent)
    : QObject{parent},
      window(w),
      model(m)
{
    w->setModel(m);
}

ACController::~ACController(){}

void ACController::moduleClicked(ACObjectItem *moduleItem)
{
    window->showModuleMenu(moduleItem);
}

void ACController::onInterfaceClicked(ACLocalApplication *app)
{
    QProcess *proc = new QProcess(this);
    proc->start(app->m_exec, QStringList());
}
