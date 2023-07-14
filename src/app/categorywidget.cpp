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

#include "../core/enums.h"

CategoryWidget::CategoryWidget(QStandardItem *item, QWidget *parent)
    : QWidget{parent},
      ui(new Ui::CategoryWidget)
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

    QVariantMap catDisplay = item->data(Qt::DisplayRole).toMap();
    QVariantMap catDescription = item->data(UserRoles::DescriptionRole).toMap();
    ui->headerLabel->setText(
                catDisplay.contains(language) ?
                    catDisplay[language].toString() :
                    catDisplay["Default"].toString());
    ui->descriptionLabel->setText(
                catDescription.contains(language) ?
                    catDescription[language].toString() :
                    catDescription["Default"].toString());
    QPixmap iconMap = item->data(Qt::DecorationRole).value<QPixmap>();
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
    ui->descriptionLabel->setMinimumSize(ui->descriptionLabel->sizeHint());
    ui->headerWidget->setLayout(ihdLayout);
    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());
    ui->modulesWidget->setLayout(modulesLayout);

    for (int i = 0; i < m->rowCount(item->index()); ++i){
        QStandardItem *moduleItem = item->child(i);
        QVariantMap moduleDisplay = moduleItem->data(Qt::DisplayRole).toMap();
        QString text =
                moduleDisplay.contains(language) ?
                    moduleDisplay[language].toString() :
                    moduleDisplay["Default"].toString();
        QPushButton *moduleButton = new QPushButton();
        moduleButton->setText(text);
        moduleButton->setMinimumWidth(moduleButton->sizeHint().width());

        QMenu *moduleMenu = new QMenu(moduleButton);
        for (int j = 0; j < m->rowCount(moduleItem->index()); ++j){
            QStandardItem *ifaceItem = moduleItem->child(j);
            QAction *ifaceAction = new QAction(
                        "&" + ifaceItem->data(Qt::DisplayRole).toString(),
                        moduleMenu);

            moduleMenu->addAction(ifaceAction);
        }
        moduleButton->setMenu(moduleMenu);

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
