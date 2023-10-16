#ifndef OBJECTBUILDERFACTORY_H
#define OBJECTBUILDERFACTORY_H

#include <memory>

#include "desktopfileparser.h"
#include "objectbuilderinterface.h"

namespace ab
{
namespace model
{
class ObjectBuilderFactory
{
    static const QString ALTERATOR_SECTION_NAME;
    static const QString DESKTOP_SECTION_NAME;

public:
    static std::unique_ptr<ObjectBuilderInterface> getBuilder(DesktopFileParser *parser);
};

} // namespace model
} // namespace ab

#endif // OBJECTBUILDERFACTORY_H
