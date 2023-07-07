#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "categorywidget.h"

#include <QMainWindow>
#include <QStandardItemModel>

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

private:

    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&)      = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

    Ui::MainWindow *ui;

    QStandardItemModel *model;
};
#endif // MAINWINDOW_H
