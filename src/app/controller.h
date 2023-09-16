#ifndef AB_CONTROLLER_H
#define AB_CONTROLLER_H

#include "model/model.h"
#include "model/objectitem.h"
#include "pushbutton.h"

#include <utility>

#include <QObject>
#include <QPushButton>
#include <QStandardItemModel>

namespace ab
{
class CategoryWidget;
class MainWindow;

class ControllerPrivate;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(MainWindow *w, std::unique_ptr<model::Model> m, QObject *parent = nullptr);
    ~Controller() = default;

public slots:
    void moduleClicked(model::ObjectItem *moduleItem);
    void onInterfaceClicked(model::LocalApplication *app);
    void onDBusStructureUpdate(QString service, QString prev, QString next);

private:
    ControllerPrivate *d;

    Controller(const Controller &) = delete;
    Controller(Controller &&)      = delete;
    Controller &operator=(const Controller &) = delete;
    Controller &operator=(Controller &&) = delete;
};
} // namespace ab

#endif // AB_CONTROLLER_H
