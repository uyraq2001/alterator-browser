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
      model(m),
      categoryDataItems(QMap<CategoryWidget *, ACObjectItem *>()),
      moduleDataItems(QMap<ACPushButton *, ACObjectItem *>())
{
    w->setModel(m);
}

ACController::~ACController(){}

void ACController::moduleClicked(ACPushButton *module)
{
    if (module == nullptr){return;}
    ACObjectItem *item = moduleDataItems.value(module);
    QMenu *menu = new QMenu(module);
    module->setMenu(menu);
    for (auto i: item->m_acObject.get()->m_interfaces){
        QAction *interfaceAction = new QAction("&" + i, menu);
        menu->addAction(interfaceAction);
        qWarning() << "action " + i + " set!";
    }
    qWarning() << "module " + module->text() +
                  " with data " +
                  item->m_acObject.get()->m_displayName +
                  " have been clicked and regitred by controller!";
}
