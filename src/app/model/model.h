#ifndef AB_MODEL_MODEL_H
#define AB_MODEL_MODEL_H

#include <memory>
#include <QStandardItemModel>
#include <QtPlugin>

#include "../../aobuilder/datasource/datasourceinterface.h"
#include "../../aobuilder/objects/category.h"
#include "item.h"
#include "modelinterface.h"

namespace ab::model
{
class Model : public QStandardItemModel, public ab::model::ModelInterface
{
    Q_OBJECT
    Q_INTERFACES(ab::model::ModelInterface)
public:
    Model();
    ~Model() override;

    // Categories
    std::vector<ao_builder::Id> getCategories() override;
    std::optional<ao_builder::Category> getCategory(ao_builder::Id id) override;

    // Applications
    std::vector<ao_builder::Id> getLocalApplications() override;
    std::optional<ao_builder::LocalAppObject> getLocalApplication(QString id) override;
    std::set<QString> getInterfaces() override;
    std::vector<ao_builder::Id> getLocalApplicationsByInterface(QString iface) override;

    // Legacy Objects
    std::vector<ao_builder::Id> getLegacyObjects() override;
    std::optional<ao_builder::LegacyObject> getLegacyObject(ao_builder::Id id) override;
    std::vector<ao_builder::Id> getLegacyObjectsByInterface(QString iface) override;
    std::vector<ao_builder::Id> getLegacyObjectsByCategory(ao_builder::Id category_id) override;

    void translateModel(QString locale) override;

public:
    Model(const Model &) = delete;
    Model(Model &&)      = delete;
    Model &operator=(const Model &) = delete;
    Model &operator=(Model &&) = delete;

private:
    void translateItem(QStandardItem *item, QString locale);
    void build();

private:
    std::unique_ptr<ModelItem> categoriesRoot{};
    std::unique_ptr<ModelItem> appsRoot{};
    std::unique_ptr<ModelItem> legacyObjectsRoot{};

    std::unique_ptr<ao_builder::DataSourceInterface> updateSource{};

signals:
    void modelUpdated();

private slots:
    void updateModel();
};
} // namespace ab::model

#endif // AB_MODEL_MODEL_H
