#ifndef AB_CONTROLLER_H
#define AB_CONTROLLER_H

#include "../aobuilder/builders/aobuilderinterface.h"
#include "../aobuilder/constants.h"
#include "../aobuilder/datasource/datasourceinterface.h"
#include "../aobuilder/objects/localappobject.h"
#include "model/model.h"
#include "model/modelinterface.h"

#include <memory>
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
    explicit Controller(std::shared_ptr<MainWindow>,
                        std::unique_ptr<model::ModelInterface>,
                        std::unique_ptr<ao_builder::DataSourceInterface>,
                        std::unique_ptr<ao_builder::AOBuilderInterface>,
                        QObject *parent = nullptr);
    ~Controller() override;

public:
    Controller(const Controller &)            = delete;
    Controller(Controller &&)                 = delete;
    Controller &operator=(const Controller &) = delete;
    Controller &operator=(Controller &&)      = delete;

public slots:
    void moduleClicked(ao_builder::Object obj);
    //    void onInterfaceClicked(ao_builder::LocalAppObject *app);
    //    void onDBusStructureUpdate(QString service, QString prev, QString next);

private:
    void translateModel();
    void buildModel();

private:
    ControllerPrivate *d;
};
} // namespace ab

#endif // AB_CONTROLLER_H
