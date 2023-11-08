#ifndef AB_MODEL_OBJECT_BUILDER_INTERFACE_H
#define AB_MODEL_OBJECT_BUILDER_INTERFACE_H

#include "desktopfileparser.h"
#include "object.h"

#include <memory>

namespace ab
{
namespace model
{
class ObjectBuilderInterface
{
public:
    virtual ~ObjectBuilderInterface() = default;

    virtual std::unique_ptr<Object> buildObject(DesktopFileParser *infoParser) = 0;
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_OBJECT_BUILDER_INTERFACE_H
