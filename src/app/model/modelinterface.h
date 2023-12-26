#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <memory>
#include <optional>
#include <set>
#include <vector>

#include "../../aobuilder/objects/category.h"
#include "../../aobuilder/objects/localappobject.h"
#include "../../aobuilder/objects/object.h"

namespace ab::model
{
class ModelInterface
{
public:
    virtual ~ModelInterface() = default;

public:
    virtual std::vector<ao_builder::Id> getCategories()                        = 0;
    virtual std::optional<ao_builder::Category> getCategory(ao_builder::Id id) = 0;
    virtual std::optional<ao_builder::Category> getDefaultCategory()           = 0;

    virtual std::vector<ao_builder::Id> getLocalApplications()                               = 0;
    virtual std::optional<ao_builder::LocalAppObject> getLocalApplication(ao_builder::Id id) = 0;
    virtual std::set<QString> getInterfaces()                                                = 0;
    virtual std::vector<QString> getLocalApplicationsByInterface(QString iface)              = 0;

    virtual std::vector<ao_builder::Id> getObjects()                              = 0;
    virtual std::optional<ao_builder::Object> getObject(ao_builder::Id id)        = 0;
    virtual std::vector<ao_builder::Id> getObjectsByInterface(QString iface)      = 0;
    virtual std::vector<QString> getObjectsByCategory(ao_builder::Id category_id) = 0;

    virtual void translateModel(QString locale) = 0;
    virtual void build(std::vector<std::unique_ptr<ao_builder::Object>> categories,
                       std::vector<std::unique_ptr<ao_builder::Object>> apps,
                       std::vector<std::unique_ptr<ao_builder::Object>> objects)
        = 0;
};
} // namespace ab::model

#endif // MODELINTERFACE_H
