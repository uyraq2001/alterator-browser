#include "mainwindow.h"
#include "categorywidget.h"
#include "controller.h"
#include "mainwindowsettings.h"
#include "ui_infodialog.h"
#include "ui_mainwindow.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomText>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QShortcut>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>

#include <map>
#include <memory>
#include <vector>

namespace ab
{
class MainWindowPrivate
{
public:
    std::unique_ptr<Ui::MainWindow> ui           = nullptr;
    std::unique_ptr<MainWindowSettings> settings = nullptr;
    model::ModelInterface *model                 = nullptr;
    Controller *controller                       = nullptr;
    CategoryWidget *defaultCategory              = nullptr;
    QShortcut *quitShortcut                      = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
{
    d->ui = std::make_unique<Ui::MainWindow>();
    d->ui->setupUi(this);
    d->quitShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));

    d->settings = std::make_unique<MainWindowSettings>(this, d->ui.get());
    d->settings->restoreSettings();

    auto categoryLayout = std::make_unique<QVBoxLayout>();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    d->ui->scrollArea->widget()->setLayout(categoryLayout.release());

    auto toolBar      = new QToolBar(this);
    auto infoButton   = new QToolButton(toolBar);
    auto switchButton = new QToolButton(toolBar);

    infoButton->setText(tr("Info"));
    infoButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    auto infoIcon = QIcon::fromTheme("help");
    infoButton->setIcon(infoIcon);

    auto infoWindow = new QDialog(this);
    Ui::InfoDialog dialogUi;
    dialogUi.setupUi(infoWindow);
    connect(infoButton, &QToolButton::clicked, this, [infoWindow](bool) { infoWindow->show(); });

    switchButton->setText(tr("Switch to older version"));
    switchButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    auto switchIcon = QIcon::fromTheme("reload");
    switchButton->setIcon(switchIcon);
    connect(switchButton, &QToolButton::clicked, this, [this](bool) { d->controller->switchBack(); });

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);
    toolBar->addWidget(switchButton);
    toolBar->addWidget(infoButton);
    toolBar->setMaximumHeight(34);
    d->ui->centralLayout->addWidget(toolBar);
    d->ui->centralLayout->addWidget(d->ui->scrollArea);

    setWindowTitle(tr("Alterator Browser"));
    setWindowIcon(QIcon(":logo.png"));
}

MainWindow::~MainWindow()
{
    delete d;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QWidget *scrollWidget = d->ui->scrollArea->widget();
    scrollWidget->setMinimumWidth(scrollWidget->layout()->minimumSize().width());
    d->ui->scrollArea->setMinimumWidth(scrollWidget->minimumWidth());
    this->setMinimumWidth(d->ui->scrollArea->minimumWidth());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    d->settings->saveSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::setController(Controller *newContoller)
{
    d->controller = newContoller;
}

void MainWindow::setModel(model::ModelInterface *newModel)
{
    d->model           = newModel;
    d->defaultCategory = nullptr;

    QLayout *categoryLayout = d->ui->scrollArea->widget()->layout();

    const std::vector<ao_builder::Id> categoriesIds = d->model->getCategories();
    const std::vector<ao_builder::Id> objectsIds    = d->model->getObjects();

    std::vector<ao_builder::Object *> objects{};
    for (const auto &objectId : objectsIds)
    {
        objects.push_back(d->model->getObject(objectId));
    }
    std::sort(objects.begin(), objects.end(), [](ao_builder::Object *a, ao_builder::Object *b) {
        return a->m_displayName < b->m_displayName;
    });

    std::map<ao_builder::Id, std::unique_ptr<CategoryWidget>> categoryWidgetMap{};

    for (const auto &object : objects)
    {
        // Case 1: category already in the map
        const auto findCategory = categoryWidgetMap.find(object->m_categoryId);
        if (findCategory != categoryWidgetMap.end())
        {
            findCategory->second->addObject(object);
            continue;
        }

        // Case 2: category not in map but exists in model
        const auto find = std::find_if(categoriesIds.begin(),
                                       categoriesIds.end(),
                                       [&object](const ao_builder::Id &category) {
                                           return category == object->m_categoryId;
                                       });
        if (find != categoriesIds.end())
        {
            const auto newCategory = d->model->getCategory(object->m_categoryId);
            auto newWidget         = std::make_unique<CategoryWidget>(this, d->model, newCategory);
            newWidget->addObject(object);
            categoryWidgetMap[object->m_categoryId] = std::move(newWidget);
            continue;
        }

        // Case 3: default category
        if (!categoryWidgetMap.count(ao_builder::DEFAULT_CATEGORY_NAME))
        {
            const auto defaultCategory = d->model->getDefaultCategory();
            auto defaultCategoryWidget = std::make_unique<CategoryWidget>(this, d->model, defaultCategory);
            categoryWidgetMap[ao_builder::DEFAULT_CATEGORY_NAME] = std::move(defaultCategoryWidget);
        }
        categoryWidgetMap[ao_builder::DEFAULT_CATEGORY_NAME]->addObject(object);
    }

    std::vector<std::unique_ptr<CategoryWidget>> categoryWidgets{};
    for (auto &[_, categoryWidget] : categoryWidgetMap)
    {
        if (!categoryWidget->isEmpty())
        {
            categoryWidgets.push_back(std::move(categoryWidget));
        }
    }
    std::sort(categoryWidgets.begin(),
              categoryWidgets.end(),
              [](const std::unique_ptr<CategoryWidget> &a, const std::unique_ptr<CategoryWidget> &b) {
                  return a->getWeight() > b->getWeight();
              });

    for (auto &categoryWidget : categoryWidgets)
    {
        categoryLayout->addWidget(categoryWidget.release());
    }
}

void MainWindow::clearUi()
{
    QLayout *categoryLayout = d->ui->scrollArea->widget()->layout();
    while (categoryLayout->itemAt(0) != nullptr)
    {
        QWidget *categoryWidget = categoryLayout->itemAt(0)->widget();
        categoryLayout->removeWidget(categoryWidget);
        delete categoryWidget;
    }
}

void MainWindow::onModuleClicked(PushButton *button)
{
    d->controller->moduleClicked(button->getObject());
}

void MainWindow::showInfo() {}
} // namespace ab
