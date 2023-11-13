#include "categorywidget.h"
#include "category.h"
#include "flowlayout.h"
#include "pushbutton.h"
#include "ui_categorywidget.h"

#include <variant>
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QSpacerItem>
#include <QStandardItemModel>

namespace ab
{
const QString INGNORE_UI       = "html";
const QString INGNORE_CATEGORY = "X-Alterator-Hidden";

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

unsigned int CategoryWidget::setItem(model::ObjectItem *newItem)
{
    this->item = newItem;

    unsigned int addedWidgets = 0;

    try
    {
        ab::model::Category currentCategory = std::get<ab::model::Category>(*newItem->getObject());

        QPixmap iconMap("/usr/share/alterator/design/images/" + currentCategory.m_icon + ".png");
        ui->iconLabel->setPixmap(iconMap);
        ui->iconLabel->setMinimumSize(iconMap.size());

        ui->titleLabel->setText(currentCategory.m_name);
        ui->titleLabel->setMinimumSize(ui->titleLabel->sizeHint());

        ui->descriptionLabel->setText(currentCategory.m_comment);

        ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());
    }
    catch (const std::bad_variant_access &e)
    {
        qCritical() << "ERROR: the item is not of Category type";
        return 0;
    }

    const int margin            = 0;
    const int horizontalSpacing = 0;
    const int verticalSpacing   = 0;
    auto modulesLayout          = std::make_unique<FlowLayout>(margin, horizontalSpacing, verticalSpacing);
    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    for (int i = 0; i < newItem->model()->rowCount(newItem->index()); ++i)
    {
        auto child = dynamic_cast<model::ObjectItem *>(newItem->child(i));
        if (!child)
        {
            qWarning() << "Can't cast item to ObjectItem to make widget!";
            continue;
        }

        std::variant<ab::model::Object, ab::model::Category> *childItem = child->getObject();

        try
        {
            //            ab::model::Object childObject = std::get<ab::model::Object>(*childItem);
            //            if (childObject.m_x_Alterator_UI == INGNORE_UI)
            //            {
            //                qWarning() << "Ignoring object with html UI:" << childObject.m_id;
            //                continue;
            //            }

            //            if (childObject.m_categoryId == INGNORE_CATEGORY)
            //            {
            //                qWarning() << "Ignoring object with hidden category:" << childObject.m_id;
            //                continue;
            //            }
        }
        catch (std::bad_variant_access const &ex)
        {
            qWarning() << "Exception: Can't cast item to ObjectItem to make widget!";
            continue;
        }

        auto moduleButton = std::make_unique<PushButton>(window);
        moduleButton->setItem(child);
        moduleButton->setFlat(true);
        modulesLayout->addWidget(moduleButton.release());
        addedWidgets++;
    }

    ui->modulesWidget->setLayout(modulesLayout.release());

    return addedWidgets;
}
} // namespace ab
