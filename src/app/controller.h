#ifndef AB_CONTROLLER_H
#define AB_CONTROLLER_H

#include "../aobuilder/objects/localappobject.h"
#include "model/model.h"
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
    ~Controller();

public:
    Controller(const Controller &) = delete;
    Controller(Controller &&)      = delete;
    Controller &operator=(const Controller &) = delete;
    Controller &operator=(Controller &&) = delete;

private:
    void translateModel();

public slots:
    void moduleClicked(ao_builder::LegacyObject obj);
    void onModelUpdated();

private:
    ControllerPrivate *d;
};
} // namespace ab

#endif // AB_CONTROLLER_H
