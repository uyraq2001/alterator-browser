#include "categorywidget.h"
#include "flowlayout.h"
#include "model/category.h"
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
    , item(nullptr)
    , window(w)
{
    ui->setupUi(this);
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

void CategoryWidget::setItem(model::ObjectItem *newItem)
{
    this->item = newItem;

    QPixmap iconMap("/usr/share/alterator/design/images/" + std::get<ab::model::Category>(newItem->getObject()).m_icon
                    + ".png");
    ui->iconLabel->setPixmap(iconMap);
    ui->iconLabel->setMinimumSize(iconMap.size());

    ui->titleLabel->setText(std::get<ab::model::Category>(newItem->getObject()).m_name);
    ui->titleLabel->setMinimumSize(ui->titleLabel->sizeHint());

    ui->descriptionLabel->setText(std::get<ab::model::Category>(newItem->getObject()).m_comment);

    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());

    auto modulesLayout = std::make_unique<FlowLayout>(10, 10, 10);
    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    for (int i = 0; i < newItem->model()->rowCount(newItem->index()); ++i)
    {
        auto moduleButton = std::make_unique<PushButton>(window);
        moduleButton->setItem(dynamic_cast<model::ObjectItem *>(newItem->child(i)));
        modulesLayout->addWidget(moduleButton.release());
    }

    ui->modulesWidget->setLayout(modulesLayout.release());
}
} // namespace ab
