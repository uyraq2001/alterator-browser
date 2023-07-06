#include "categorywidget.h"
#include "ui_categorywidget.h"
#include "flowlayout.h"
#include "modulepushbutton.h"

#include <QStandardItemModel>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>

#include "../core/enums.h"

CategoryWidget::CategoryWidget(QStandardItemModel *m, int row, QWidget *parent)
    : QWidget{parent},
      ui(new Ui::CategoryWidget)
{
    ui->setupUi(this);
    QLayout *ihdLayout = ui->headerWidget->layout();
    QLayout *ihdmLayout = this->layout();
    FlowLayout *modulesLayout = new FlowLayout(10, 10, 10);

    ihdmLayout->addWidget(ui->headerWidget);
    ihdmLayout->addWidget(ui->modulesWidget);
    ihdmLayout->addWidget(ui->bottomLine);
    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->headerLabel->setText(m->index(row, 0).data(Qt::DisplayRole).toString());
    ui->descriptionLabel->setText(m->index(row, 0).data(UserRoles::DescriptionRole).toString());
    QPixmap iconMap = m->index(row, 0).data(Qt::DecorationRole).value<QPixmap>();
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
    ui->descriptionLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->headerWidget->setLayout(ihdLayout);
    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());
    ui->modulesWidget->setLayout(modulesLayout);

    for (int i = 0; i < m->rowCount(m->index(row, 0)); ++i){
        QString text = m->index(i, 0, m->index(row, 0)).data(Qt::DisplayRole).toString();
        QMap<QString, QVariant> ifaceData = m->index(i, 0, m->index(row, 0)).data(UserRoles::ActionRole).toMap();
        ModulePushButton *moduleButton = new ModulePushButton();
        moduleButton->setText(text);
        moduleButton->setDBusInterface(ifaceData.value("service").toString(),
                                       ifaceData.value("path").toString(),
                                       ifaceData.value("interface").toString(),
                                       ifaceData.value("bus").toString() == "sessionBus"?
                                           QDBusConnection::sessionBus():
                                           QDBusConnection::systemBus());
        moduleButton->setMode(AlteratorModes::StandardMode);
        moduleButton->setMinimumWidth(moduleButton->sizeHint().width());
        modulesLayout->addWidget(moduleButton);
    }
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}
