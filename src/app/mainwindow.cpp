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

void MainWindow::setModel(model::ModelInterface &newModel)
{
    d->model                        = &newModel;
    QLayout *categoryLayout         = d->ui->scrollArea->widget()->layout();
    std::vector<QString> categories = d->model->getCategories();
    for (auto name : categories)
    {
        auto cat = d->model->getCategory(name);
        if (!cat.has_value())
        {
            qWarning() << name << ": no such category";
            continue;
        }
        auto categoryWidget = new CategoryWidget(this, d->model);

        if (categoryWidget->setCategory(cat.value()) > 0)
        {
            categoryLayout->addWidget(categoryWidget);
        }
        else
        {
            qWarning() << "Ignoring empty category!";
        }
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
