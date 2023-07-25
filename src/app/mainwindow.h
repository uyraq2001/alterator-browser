#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <utility>

#include "accontroller.h"
#include "categorywidget.h"
#include "model/acmodel.h"
#include "acpushbutton.h"
#include "mainwindowsettings.h"

class CategoryWidget;
class ACController;
class MainWindowSettings;
class ACPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void setController(ACController *c);

    bool eventFilter(QObject *watched, QEvent *event);

    void setModel(ACModel *m);
    void clearUi();

    void showModuleMenu(ACObjectItem *item);
    void onModuleClicked(ACPushButton *button);
    void onInterfaceClicked(ACLocalApplication *app);

private:

    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&)      = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

    Ui::MainWindow *ui;
    QStandardItemModel *model;
    ACController *controller;
    std::unique_ptr<MainWindowSettings> settings;

signals:

    void showMenu(ACObjectItem *item);
};
#endif // MAINWINDOW_H
