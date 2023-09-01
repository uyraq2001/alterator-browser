#include "mainwindow.h"
#include "categorywidget.h"
#include "controller.h"
#include "mainwindowsettings.h"
#include "model/objectitem.h"
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
class MainWindowPrivate{
public:
    MainWindowPrivate(MainWindow *window)
        : ui(new Ui::MainWindow)
        , quitShortcut(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), window, SLOT(close())))
        , model(nullptr)
        , controller(nullptr)
        , settings(std::make_unique<MainWindowSettings>(window, ui)){}

    ~MainWindowPrivate()
    {
        delete ui;
        controller.release();
        model.release();
    }

    MainWindowPrivate(const MainWindowPrivate &) = delete;
    MainWindowPrivate(MainWindowPrivate &&)      = delete;
    MainWindowPrivate &operator=(const MainWindowPrivate &) = delete;
    MainWindowPrivate &operator=(MainWindowPrivate &&) = delete;

    Ui::MainWindow *ui = nullptr;
    QShortcut *quitShortcut = nullptr;
    std::unique_ptr<QStandardItemModel> model = nullptr;
    std::unique_ptr<Controller> controller = nullptr;
    std::unique_ptr<MainWindowSettings> settings = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    d->ui->setupUi(this);
    d->settings->restoreSettings();

    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    d->ui->scrollArea->widget()->setLayout(categoryLayout);

    setWindowTitle(tr("Alterator Browser"));
    setWindowIcon(QIcon(":/logo.png"));
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
    d->controller = std::unique_ptr<Controller>(newContoller);
}

void MainWindow::setModel(model::Model *newModel)
{
    d->model = std::unique_ptr<model::Model>(newModel);
    QLayout *categoryLayout = d->ui->scrollArea->widget()->layout();
    for (int i = 0; i < d->model->rowCount(); ++i)
    {
        CategoryWidget *categoryWidget = new CategoryWidget(this);
        categoryLayout->addWidget(categoryWidget);
        categoryWidget->setItem(dynamic_cast<model::ObjectItem *>(d->model->item(i)));
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
    d->controller->moduleClicked(button->getItem());
}

void MainWindow::showModuleMenu(model::ObjectItem *item)
{
    emit showMenu(item);
}

void MainWindow::onInterfaceClicked(model::LocalApplication *app)
{
    d->controller->onInterfaceClicked(app);
}
} // namespace ab
