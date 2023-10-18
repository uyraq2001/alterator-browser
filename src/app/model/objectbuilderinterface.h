#ifndef OBJECTBUILDERINTERFACE_H
#define OBJECTBUILDERINTERFACE_H

#include "category.h"
#include "desktopfileparser.h"
#include "object.h"

#include <memory>
#include <variant>

namespace ab
{
namespace model
{
class ObjectBuilderInterface
{
public:
    virtual ~ObjectBuilderInterface() = default;

    virtual std::unique_ptr<Object> buildAll(DesktopFileParser *infoParser) = 0;
};
} // namespace model
} // namespace ab

#endif // OBJECTBUILDERINTERFACE_H
