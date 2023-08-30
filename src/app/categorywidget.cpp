#include "categorywidget.h"
#include "flowlayout.h"
#include "pushbutton.h"
#include "ui_categorywidget.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QSpacerItem>
#include <QStandardItemModel>

namespace ab
{
CategoryWidget::CategoryWidget(MainWindow *w, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::CategoryWidget)
    , data(nullptr)
    , window(w)
{
    ui->setupUi(this);
}

void CategoryWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

void CategoryWidget::setItem(model::ObjectItem *item)
{
    data = item;

    QLayout *ihdLayout        = ui->headerWidget->layout();
    QLayout *ihdmLayout       = this->layout();
    FlowLayout *modulesLayout = new FlowLayout(10, 10, 10);

    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->headerLabel->setText(item->getACObject()->m_categoryObject->m_name);
    ui->descriptionLabel->setText(item->getACObject()->m_categoryObject->m_comment);
    QPixmap iconMap("/usr/share/alterator/design/images/" + item->getACObject()->m_categoryObject->m_icon + ".png");
    ui->iconLabel->setPixmap(iconMap);
    ui->headerLabel->setMinimumSize(ui->headerLabel->sizeHint());
    ui->iconLabel->setMinimumSize(iconMap.size());
    ui->headerWidget->setLayout(ihdLayout);
    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());
    ui->modulesWidget->setLayout(modulesLayout);

    for (int i = 0; i < item->model()->rowCount(item->index()); ++i)
    {
        PushButton *moduleButton = new PushButton(window);
        modulesLayout->addWidget(moduleButton);
        moduleButton->setItem(dynamic_cast<model::ObjectItem *>(item->child(i)));
    }
}
} // namespace ab
