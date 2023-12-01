#ifndef AB_MODEL_MODEL_H
#define AB_MODEL_MODEL_H

#include <memory>
#include <QStandardItemModel>

#include "../../aobuilder/objects/category.h"
#include "objectitem.h"

namespace ab::model
{
class Model : public QStandardItemModel
{
public:
    Model();
    ~Model() override;

    // Categories
    std::vector<ao_builder::Id> getCategories();
    std::optional<ao_builder::Category> getCategory(ao_builder::Id category_id);

    void translateModel(QString locale);

public:
    Model(const Model &)            = delete;
    Model(Model &&)                 = delete;
    Model &operator=(const Model &) = delete;
    Model &operator=(Model &&)      = delete;

private:
    void translateItem(QStandardItem *item, QString locale);

private:
    std::unique_ptr<ModelItem> categoriesRoot{};
    std::unique_ptr<ModelItem> appsRoot{};
    std::unique_ptr<ModelItem> legacyObjectsRoot{};
};
} // namespace ab::model

#endif // AB_MODEL_MODEL_H
