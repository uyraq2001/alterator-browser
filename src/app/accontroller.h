#ifndef ACCONTROLLER_H
#define ACCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <utility>
#include <QPushButton>

#include "mainwindow.h"

#include "acpushbutton.h"
#include "categorywidget.h"
#include "model/acmodel.h"

class CategoryWidget;
class MainWindow;

class ACController : public QObject
{
    Q_OBJECT
public:
    explicit ACController(MainWindow *w, ACModel *m, QObject *parent = nullptr);
    ~ACController();

    void moduleClicked(ACObjectItem *moduleItem);
    void onInterfaceClicked(ACApplication *app);

private:

    MainWindow *window;
    QStandardItemModel *model;

    ACController(const ACController &) = delete;
    ACController(ACController &&)      = delete;
    ACController &operator=(const ACController &) = delete;
    ACController &operator=(ACController &&) = delete;
};

#endif // ACCONTROLLER_H
