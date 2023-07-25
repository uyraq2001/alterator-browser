#include "accontroller.h"

#include <QAction>
#include <QDebug>
#include <QLayout>
#include <QMenu>
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
    window->showModuleMenu(moduleItem);
}
