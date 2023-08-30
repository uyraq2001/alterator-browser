#ifndef ACCONTROLLER_H
#define ACCONTROLLER_H

#include <utility>
#include <QObject>
#include <QPushButton>
#include <QStandardItemModel>

#include "mainwindow.h"

#include "pushbutton.h"
#include "categorywidget.h"
#include "model/model.h"

class CategoryWidget;
class MainWindow;

class ACController : public QObject
{
    Q_OBJECT
public:
    explicit ACController(MainWindow *w, std::unique_ptr<ACModel> m, QObject *parent = nullptr);
    ~ACController();

public slots:

    void moduleClicked(ACObjectItem *moduleItem);
    void onInterfaceClicked(ACLocalApplication *app);
    void onDBusStructureUpdate(QString service, QString prev, QString next);

private:
    MainWindow *window;
    std::unique_ptr<ACModel> model;

    ACController(const ACController &) = delete;
    ACController(ACController &&)      = delete;
    ACController &operator=(const ACController &) = delete;
    ACController &operator=(ACController &&) = delete;
};

#endif // ACCONTROLLER_H
