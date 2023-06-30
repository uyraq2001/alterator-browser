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
    QHBoxLayout *ihdLayout = new QHBoxLayout();
    QLayout *ihdmLayout = this->layout();
    hdLayout->addWidget(ui->headerLabel);
    hdLayout->addWidget(ui->descriptionLabel);
    hdLayout->setAlignment(Qt::AlignTop);
    ihdLayout->addWidget(ui->iconLabel);
    ihdLayout->addLayout(hdLayout);
    ihdLayout->setAlignment(Qt::AlignLeft);
    ihdmLayout->setAlignment(Qt::AlignTop);
//    ihdmLayout->setSizeConstraint(QLayout::SetFixedSize);

//    ui->headerLabel->setText("krb svdkjvwrgnwrmjbnergr,j,bmejbhrs bgmesjdgierwfbveibvi4enfej,snvwenfv m,fdv m,oino");
    ui->headerLabel->setText(m->index(row, 0).data(Qt::DisplayRole).toString());
    QPixmap iconMap = QPixmap("/usr/share/app-info/icons/altlinux/64x64/adanaxisgpl.png");
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
//    ui->iconLabel->setMinimumSize(QSize(0, 0));
    ui->descriptionLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->headerWidget->setMinimumSize(ui->headerWidget->sizeHint());
    ui->headerWidget->setLayout(ihdLayout);
    ihdmLayout->addWidget(ui->headerWidget);

    FlowLayout *modulesLayout = new FlowLayout(10, 10, 10);
    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    ui->modulesWidget->setLayout(modulesLayout);

    ihdmLayout->addWidget(ui->modulesWidget);
    for (int i = 0; i < m->rowCount(m->index(row, 0)); ++i){
        QString text = m->index(i, 0, m->index(row, 0)).data(Qt::DisplayRole).toString();
        QPushButton *moduleButton = new QPushButton(text, ui->modulesWidget);
        moduleButton->setMinimumWidth(moduleButton->sizeHint().width());
        modulesLayout->addWidget(moduleButton);
    }
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

//    QPalette pal = QPalette();

//    // set black background
//    // Qt::black / "#000000" / "black"
//    pal.setColor(QPalette::Window, Qt::red);

//    ui->headerWidget->setAutoFillBackground(true);
//    ui->headerWidget->setPalette(pal);
//    ui->headerWidget->show();
//    pal.setColor(QPalette::Window, Qt::green);

//    ui->headerLabel->setAutoFillBackground(true);
//    ui->headerLabel->setPalette(pal);
//    ui->headerLabel->show();
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}
