#ifndef AB_MODEL_MODEL_H
#define AB_MODEL_MODEL_H

#include <memory>
#include <QStandardItemModel>

#include "../../aobuilder/objects/category.h"
#include "item.h"
#include "modelinterface.h"

namespace ab::model
{
class Model : public QStandardItemModel, public ab::model::ModelInterface
{
public:
    Model();
    ~Model() override = default;

    // Categories
    std::vector<ao_builder::Id> getCategories() override;
    std::optional<ao_builder::Category> getCategory(ao_builder::Id id) override;
    std::optional<ao_builder::Category> getDefaultCategory() override;

    // Applications
    std::vector<ao_builder::Id> getLocalApplications() override;
    std::optional<ao_builder::LocalAppObject> getLocalApplication(QString id) override;
    std::set<QString> getInterfaces() override;
    std::vector<ao_builder::Id> getLocalApplicationsByInterface(QString iface) override;

    // Objects
    std::vector<ao_builder::Id> getObjects() override;
    std::optional<ao_builder::Object> getObject(ao_builder::Id id) override;
    std::vector<ao_builder::Id> getObjectsByInterface(QString iface) override;
    std::vector<ao_builder::Id> getObjectsByCategory(ao_builder::Id category_id) override;

    void translateModel(QString locale) override;
    void clear();
    void build(std::vector<std::unique_ptr<ao_builder::Object>> categories,
               std::vector<std::unique_ptr<ao_builder::Object>> apps,
               std::vector<std::unique_ptr<ao_builder::Object>> objects) override;

public:
    Model(const Model &) = delete;
    Model(Model &&)      = delete;
    Model &operator=(const Model &) = delete;
    Model &operator=(Model &&)      = delete;

private:
    void translateItem(QStandardItem *item, QString locale);

private:
    ModelItem *categoriesRoot{nullptr};
    ModelItem *appsRoot{nullptr};
    ModelItem *objectsRoot{nullptr};
};
} // namespace ab::model

#endif // AB_MODEL_MODEL_H
