#ifndef AB_CONTROLLER_H
#define AB_CONTROLLER_H

#include "model/model.h"
#include "model/objectitem.h"

#include <utility>

#include <QObject>
#include <QPushButton>
#include <QStandardItemModel>

namespace ab
{
class CategoryWidget;
class MainWindow;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(MainWindow *w, std::unique_ptr<model::Model> m, QObject *parent = nullptr);
    ~Controller();

public slots:

    void moduleClicked(model::ObjectItem *moduleItem);
    void onInterfaceClicked(model::LocalApplication *app);
    void onDBusStructureUpdate(QString service, QString prev, QString next);

private:
    MainWindow *window;
    std::unique_ptr<model::Model> model;

    Controller(const Controller &) = delete;
    Controller(Controller &&)      = delete;
    Controller &operator=(const Controller &) = delete;
    Controller &operator=(Controller &&) = delete;
};
} // namespace ab

#endif // AB_CONTROLLER_H
