#ifndef OBJECTBUILDERINTERFACE_H
#define OBJECTBUILDERINTERFACE_H

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

#endif // OBJECTBUILDERINTERFACE_H
