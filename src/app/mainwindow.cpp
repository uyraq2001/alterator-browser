#include "mainwindow.h"
#include "categorywidget.h"
#include "controller.h"
#include "mainwindowsettings.h"
#include "ui_mainwindow.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomText>
#include <QLayout>
#include <QMouseEvent>
#include <QShortcut>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

namespace ab
{
class MainWindowPrivate
{
public:
    Ui::MainWindow *ui                           = nullptr;
    QShortcut *quitShortcut                      = nullptr;
    model::ModelInterface *model                 = nullptr;
    Controller *controller                       = nullptr;
    std::unique_ptr<MainWindowSettings> settings = nullptr;
    CategoryWidget *defaultCategory              = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate())
{
    d->ui           = new Ui::MainWindow;
    d->quitShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
    d->settings     = std::make_unique<MainWindowSettings>(this, d->ui);
    d->ui->setupUi(this);
    d->settings->restoreSettings();

    auto categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    d->ui->scrollArea->widget()->setLayout(categoryLayout);

    setWindowTitle(tr("Alterator Browser"));
    setWindowIcon(QIcon(":/logo.png"));
}

MainWindow::~MainWindow()
{
    delete d->ui;
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

    std::vector<ao_builder::Id> categories = d->model->getCategories();
    std::vector<ao_builder::Id> objects    = d->model->getObjects();
    QMap<ao_builder::Id, CategoryWidget *> categoryMap{};

    for (auto currentObjectId : objects)
    {
        auto currentObject = d->model->getObject(currentObjectId);
        auto catIt         = categoryMap.find(currentObject.value().m_categoryId);

        if (catIt != categoryMap.end())
        {
            catIt.value()->addObject(currentObject.value());
            continue;
        }
        else
        {
            const auto find
                = std::find_if(categories.begin(), categories.end(), [&currentObject](const ao_builder::Id &catName) {
                      return !(QString::compare(catName, currentObject.value().m_categoryId, Qt::CaseSensitive));
                  });
            if (find == categories.end())
            {
                auto defaultCatIt = categoryMap.find(ao_builder::DEFAULT_CATEGORY_NAME);
                if (defaultCatIt == categoryMap.end())
                {
                    auto *defaultCatWidget = new CategoryWidget(this, d->model, d->model->getDefaultCategory().value());

                    categoryMap.insert(ao_builder::DEFAULT_CATEGORY_NAME, defaultCatWidget);

                    defaultCatWidget->addObject(currentObject.value());
                }
                else
                {
                    defaultCatIt.value()->addObject(currentObject.value());
                }
                continue;
            }

            auto newCat = d->model->getCategory(currentObject.value().m_categoryId);
            if (newCat.has_value())
            {
                auto *newWidget = new CategoryWidget(this, d->model, newCat.value());

                newWidget->addObject(currentObject.value());

                categoryMap.insert(currentObject.value().m_categoryId, newWidget);
            }
        }
    }

    for (const auto &category : categoryMap.values())
    {
        categoryLayout->addWidget(category);
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
} // namespace ab
