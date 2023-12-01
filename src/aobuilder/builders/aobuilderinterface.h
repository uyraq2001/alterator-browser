#ifndef AOB_ALTERATOR_OBJECT_BUILDER_INTERFACE_H
#define AOB_ALTERATOR_OBJECT_BUILDER_INTERFACE_H

#include <memory>
#include <QString>

#include "../objects/object.h"

namespace ao_builder
{
class AOBuilderInterface
{
public:
    virtual ~AOBuilderInterface() = default;

public:
    virtual std::vector<std::unique_ptr<Object>> buildLocalApps()     = 0;
    virtual std::vector<std::unique_ptr<Object>> buildCategories()    = 0;
    virtual std::vector<std::unique_ptr<Object>> buildLegacyObjects() = 0;
    virtual std::vector<std::unique_ptr<Object>> buildObjects()       = 0;
    virtual std::unique_ptr<Object> buildObject(QString info)         = 0;
};
} // namespace ao_builder

#endif // AOB_ALTERATOR_OBJECT_BUILDER_INTERFACE_H
