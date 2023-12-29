#include "categorywidget.h"
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
const QString IGNORE_UI       = "html";
const QString IGNORE_CATEGORY = "X-Alterator-Hidden";

const int MARGIN             = 0;
const int HORIZONTAL_SPACING = 0;
const int VERTICAL_SPACING   = 0;

CategoryWidget::CategoryWidget(MainWindow *w, model::ModelInterface *m, ao_builder::Category *cat, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::CategoryWidget)
    , category(cat)
    , window(w)
    , model(m)
    , layout(new FlowLayout(MARGIN, HORIZONTAL_SPACING, VERTICAL_SPACING))
{
    ui->setupUi(this);

    QPixmap iconMap("/usr/share/alterator/design/images/" + cat->m_icon + ".png");
    ui->iconLabel->setPixmap(iconMap);
    ui->iconLabel->setMinimumSize(iconMap.size());

    ui->titleLabel->setText(cat->m_displayName);
    ui->titleLabel->setMinimumSize(ui->titleLabel->sizeHint());

    ui->descriptionLabel->setText(cat->m_comment);

    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());

    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ui->modulesWidget->setLayout(layout);
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

void CategoryWidget::addObject(ao_builder::Object *object)
{
    const auto legacyObject = dynamic_cast<ao_builder::LegacyObject *>(object);

    if (object->m_isLegacy && legacyObject != nullptr)
    {
        if (legacyObject->m_x_Alterator_UI == IGNORE_UI)
        {
            qInfo() << "Ignoring object with html UI:" << object->m_id;
            return;
        }

        if (legacyObject->m_categoryId == IGNORE_CATEGORY)
        {
            qInfo() << "Ignoring object with hidden category:" << object->m_id;
            return;
        }
    }

    auto moduleButton = std::make_unique<PushButton>(window);
    moduleButton->setObject(object);
    moduleButton->setFlat(true);
    layout->addWidget(moduleButton.release());
}

int CategoryWidget::getWeight()
{
    return category->m_weight;
}

bool CategoryWidget::isEmpty()
{
    return layout->isEmpty();
}
} // namespace ab
