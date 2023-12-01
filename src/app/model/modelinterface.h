#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <optional>
#include <vector>

#include "../../aobuilder/objects/category.h"
#include "../../aobuilder/objects/legacyobject.h"
#include "../../aobuilder/objects/localappobject.h"

class ModelInterface
{
    virtual std::vector<QString> getCategories();
    virtual std::optional<ao_builder::Category> getCategory(QString id);

    virtual std::vector<QString> getLocalApplications();
    virtual std::vector<QString> getLocalApplicationsByInterface(QString iface);
    virtual std::optional<ao_builder::LocalAppObject> getLocalApplication(QString id);

    virtual std::vector<QString> getObjects();
    virtual std::vector<QString> getObjectsByInterface(QString iface);
    virtual std::vector<QString> getObjectsByCategory(QString cat);
    virtual std::optional<ao_builder::LegacyObject> getObject(QString id);
};

#endif // MODELINTERFACE_H
