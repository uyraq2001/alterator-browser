#include "categorywidget.h"
#include "ui_categorywidget.h"
#include "flowlayout.h"

#include <QStandardItemModel>
#include <QPushButton>
#include <QDebug>

CategoryWidget::CategoryWidget(QStandardItemModel *m, int row, QWidget *parent)
    : QWidget{parent},
      ui(new Ui::CategoryWidget)
{
    ui->setupUi(this);
    QVBoxLayout *hdLayout = new QVBoxLayout();
    QHBoxLayout *ihdLayout = new QHBoxLayout(ui->headerWidget);
    hdLayout->addWidget(ui->headerLabel);
    hdLayout->addWidget(ui->descriptionLabel);
    ihdLayout->addWidget(ui->iconLabel);
    ihdLayout->addLayout(hdLayout);
    ihdLayout->setAlignment(Qt::AlignLeft);

//    ui->headerLabel->setText("krb svdkjvwrgnwrmjbnergr,j,bmejbhrs bgmesjdgierwfbveibvi4enfej,snvwenfv m,fdv m,dfghjkl;';lkjhgfdghjkl;oenfvoerbvelnrfjerb");
    ui->headerLabel->setText(m->index(row, 0).data(Qt::DisplayRole).toString());
    QPixmap iconMap = QPixmap("/usr/share/app-info/icons/altlinux/64x64/adanaxisgpl.png");
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
    ui->descriptionLabel->setMinimumSize(ui->headerLabel->sizeHint());

    FlowLayout *modulesLayout = new FlowLayout(10, 10, 10);
//    QGridLayout* modulesLayout = new QGridLayout();
    ui->modulesWidget->setLayout(modulesLayout);
    for (int i = 0; i < m->rowCount(m->index(row, 0)); ++i){
        QString text = m->index(i, 0, m->index(row, 0)).data(Qt::DisplayRole).toString();
        QPushButton *moduleButton = new QPushButton(text, ui->modulesWidget);
        moduleButton->setMinimumWidth(moduleButton->sizeHint().width());
        modulesLayout->addWidget(moduleButton);
    }
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

//    QPushButton *l1 = new QPushButton("sdfghjhgfdsdfcgvjhj");
//    QPushButton *l2 = new QPushButton("sdfghjhgfdsdfcgvjhj");

//    l1->setMinimumWidth(l1->sizeHint().width());
//    l2->setMinimumWidth(l2->sizeHint().width());

//    modulesLayout->addWidget(l1);
//    modulesLayout->addWidget(l2);

//    ui->modulesWidget->setMinimumSize(ui->modulesWidget->sizeHint());

}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}
