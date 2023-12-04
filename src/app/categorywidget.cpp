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

CategoryWidget::CategoryWidget(MainWindow *w, model::ModelInterface *m, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::CategoryWidget)
    , category(nullptr)
    , window(w)
    , model(m)
{
    ui->setupUi(this);
}

CategoryWidget::~CategoryWidget()
{
    delete ui;
}

unsigned int CategoryWidget::setCategory(ao_builder::Category cat)
{
    unsigned int addedWidgets = 0;

    QPixmap iconMap("/usr/share/alterator/design/images/" + cat.m_icon + ".png");
    ui->iconLabel->setPixmap(iconMap);
    ui->iconLabel->setMinimumSize(iconMap.size());

    ui->titleLabel->setText(cat.m_displayName);
    ui->titleLabel->setMinimumSize(ui->titleLabel->sizeHint());

    ui->descriptionLabel->setText(cat.m_comment);

    ui->headerWidget->setMinimumWidth(ui->headerWidget->sizeHint().width());

    const int margin            = 0;
    const int horizontalSpacing = 0;
    const int verticalSpacing   = 0;
    auto modulesLayout          = std::make_unique<FlowLayout>(margin, horizontalSpacing, verticalSpacing);
    modulesLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    modulesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    for (auto moduleName : model->getLegacyObjectsByCategory(cat.m_id))
    {
        auto module = model->getLegacyObject(moduleName);
        if (!module.has_value())
        {
            qWarning() << moduleName << ": no such module!";
            continue;
        }

        if (module.value().m_x_Alterator_UI == IGNORE_UI)
        {
            qWarning() << "Ignoring object with html UI:" << module.value().m_id;
            continue;
        }

        if (module.value().m_categoryId == IGNORE_CATEGORY)
        {
            qWarning() << "Ignoring object with hidden category:" << module.value().m_id;
            continue;
        }

        auto moduleButton = std::make_unique<PushButton>(window);
        moduleButton->setObject(module.value());
        moduleButton->setFlat(true);
        modulesLayout->addWidget(moduleButton.release());
        addedWidgets++;
    }

    ui->modulesWidget->setLayout(modulesLayout.release());

    return addedWidgets;
}
} // namespace ab
