#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "categorywidget.h"

#include <QMainWindow>
#include <QStandardItemModel>

#include "accontroller.h"

class ACController;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStandardItemModel *m, QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void setController(ACController *c);

    bool eventFilter(QObject *watched, QEvent *event);

private:

    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&)      = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

    Ui::MainWindow *ui;

    QStandardItemModel *model;

    ACController *controller;
};
#endif // MAINWINDOW_H
