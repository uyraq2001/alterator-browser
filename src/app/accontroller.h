#ifndef ACCONTROLLER_H
#define ACCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <utility>
#include <QPushButton>

#include "mainwindow.h"

class MainWindow;

class ACController : public QObject
{
    Q_OBJECT
public:
    explicit ACController(MainWindow *w, QStandardItemModel *m, QObject *parent = nullptr);
    ~ACController();

    void moduleClicked(QPushButton *module);

private:

    MainWindow *window;
    QStandardItemModel *model;
    QMap<QWidget *, ACObjectItem *> dataItems;

    ACController(const ACController &) = delete;
    ACController(ACController &&)      = delete;
    ACController &operator=(const ACController &) = delete;
    ACController &operator=(ACController &&) = delete;
};

#endif // ACCONTROLLER_H
