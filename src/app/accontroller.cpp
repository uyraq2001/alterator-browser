#include "accontroller.h"

#include <QPushButton>
#include <QLayout>
#include <QDebug>
#include <QScrollArea>
#include <QAction>
#include <QMenu>

ACController::ACController(MainWindow *w, QStandardItemModel *m, QObject *parent)
    : QObject{parent},
      window(w),
      model(m),
      dataItems(QMap<QWidget *, ACObjectItem *>())
{
    QLayout *categoryLayout = qobject_cast<QScrollArea *>(w->centralWidget()->layout()->itemAt(0)->layout()->itemAt(0)->widget())->widget()->layout();
    for (int i = 0; categoryLayout->itemAt(i) != nullptr; ++i){
        QWidget *categoryWidget = categoryLayout->itemAt(i)->widget();
        ACObjectItem *categoryItem = dynamic_cast<ACObjectItem *>(
                    model->itemFromIndex(model->index(i, 0)));
        dynamic_cast<CategoryWidget *>(categoryWidget)->setController(this);
        dataItems.insert(categoryWidget, categoryItem);

        auto t = categoryWidget->layout()->itemAt(1)->widget();
        QLayout *modulesLayout = categoryWidget->layout()->itemAt(1)->widget()->layout();
        for (int j = 0; modulesLayout->itemAt(j) != nullptr; ++j){
            QWidget *moduleWidget = modulesLayout->itemAt(j)->widget();
            ACObjectItem *moduleItem = dynamic_cast<ACObjectItem *>(
                        model->itemFromIndex(
                            model->index(i, 0, categoryItem->index())));
            dataItems.insert(moduleWidget, moduleItem);
        }
    }
}

ACController::~ACController(){}

void ACController::moduleClicked(QPushButton *module)
{
    if (module == nullptr){return;}
    ACObjectItem *item = dataItems.value(module);
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
