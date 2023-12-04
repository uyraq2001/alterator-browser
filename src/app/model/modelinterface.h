#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <optional>
#include <set>
#include <vector>

#include "../../aobuilder/objects/category.h"
#include "../../aobuilder/objects/legacyobject.h"
#include "../../aobuilder/objects/localappobject.h"

namespace ab
{
namespace model
{
class ModelInterface
{
public:
    virtual ~ModelInterface() = default;

public:
    virtual std::vector<ao_builder::Id> getCategories()                        = 0;
    virtual std::optional<ao_builder::Category> getCategory(ao_builder::Id id) = 0;

    virtual std::vector<ao_builder::Id> getLocalApplications()                               = 0;
    virtual std::optional<ao_builder::LocalAppObject> getLocalApplication(ao_builder::Id id) = 0;
    virtual std::set<QString> getInterfaces()                                                = 0;
    virtual std::vector<QString> getLocalApplicationsByInterface(QString iface)              = 0;

    virtual std::vector<ao_builder::Id> getLegacyObjects()                              = 0;
    virtual std::optional<ao_builder::LegacyObject> getLegacyObject(ao_builder::Id id)  = 0;
    virtual std::vector<ao_builder::Id> getLegacyObjectsByInterface(QString iface)      = 0;
    virtual std::vector<QString> getLegacyObjectsByCategory(ao_builder::Id category_id) = 0;
};
} // namespace model
} // namespace ab

#endif // MODELINTERFACE_H
