#include "categorywidget.h"
#include "ui_categorywidget.h"
#include "flowlayout.h"
#include "modulepushbutton.h"

#include <QStandardItemModel>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>

#include "../core/enums.h"

CategoryWidget::CategoryWidget(ACObjectItem *item, QWidget *parent)
    : QWidget{parent},
      ui(new Ui::CategoryWidget),
      controller(nullptr)
{
    QStandardItemModel *m = item->model();
    QLocale locale;
    QString language = locale.system().name().split("_").at(0);

    ui->setupUi(this);
    QLayout *ihdLayout = ui->headerWidget->layout();
    QLayout *ihdmLayout = this->layout();
    FlowLayout *modulesLayout = new FlowLayout(10, 10, 10);

    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->headerLabel->setText(item->m_acObject.get()->m_categoryObject.get()->m_name);
    ui->descriptionLabel->setText(item->m_acObject.get()->m_categoryObject.get()->m_comment);
    QPixmap iconMap("/usr/share/alterator/design/images/" +
                    item->m_acObject.get()->m_categoryObject.get()->m_icon +
                    ".png");
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
    ui->descriptionLabel->setMinimumSize(ui->descriptionLabel->sizeHint());
    ui->headerWidget->setLayout(ihdLayout);
    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());
    ui->modulesWidget->setLayout(modulesLayout);

    for (int i = 0; i < m->rowCount(item->index()); ++i){
        ACObjectItem *moduleItem = dynamic_cast<ACObjectItem *>(item->child(i));
        QVariantMap moduleDisplay = moduleItem->data(Qt::DisplayRole).toMap();
        QPushButton *moduleButton = new QPushButton();
        moduleButton->setText(moduleItem->m_acObject.get()->m_displayName);
        moduleButton->setMinimumWidth(moduleButton->sizeHint().width());
        connect(moduleButton, &QPushButton::clicked, this, &CategoryWidget::onClicked);

        QMenu *moduleMenu = new QMenu(moduleButton);
        for (int j = 0; j < m->rowCount(moduleItem->index()); ++j){
            QStandardItem *ifaceItem = moduleItem->child(j);
            QAction *ifaceAction = new QAction(
                        "&" + ifaceItem->data(Qt::DisplayRole).toString(),
                        moduleMenu);

            moduleMenu->addAction(ifaceAction);
        }
//        moduleButton->setMenu(moduleMenu);
//        moduleMenu->installEventFilter(this);

        modulesLayout->addWidget(moduleButton);
    }
}

void CategoryWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

//bool CategoryWidget::eventFilter(QObject *watched, QEvent *event)
//{
//    if (event->type() == QEvent::MouseButtonRelease){
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//        QWidget *clickedWidget = this->childAt(mouseEvent->pos());
//        controller->moduleClicked(qobject_cast<QPushButton *>(clickedWidget));
//    }
//    return QWidget::eventFilter(watched, event);
//}

void CategoryWidget::setController(ACController *c)
{
    controller = c;
}

void CategoryWidget::onClicked(bool){
    controller->moduleClicked(qobject_cast<QPushButton*>(sender()));
}
