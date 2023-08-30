#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <utility>
#include <QMainWindow>
#include <QStandardItemModel>

#include "controller.h"
#include "pushbutton.h"
#include "categorywidget.h"
#include "mainwindowsettings.h"
#include "model/model.h"

class CategoryWidget;
class Controller;
class MainWindowSettings;
class PushButton;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void setController(Controller *c);

    bool eventFilter(QObject *watched, QEvent *event);

    void setModel(Model *m);
    void clearUi();

    void showModuleMenu(ObjectItem *item);
    void onModuleClicked(PushButton *button);
    void onInterfaceClicked(LocalApplication *app);

private:
    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&)      = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Controller *controller;
    std::unique_ptr<MainWindowSettings> settings;

signals:

    void showMenu(ObjectItem *item);
};
#endif // MAINWINDOW_H
