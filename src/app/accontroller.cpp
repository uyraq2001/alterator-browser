#include "accontroller.h"

#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QScrollArea>
#include <QAction>
#include <QMenu>

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
