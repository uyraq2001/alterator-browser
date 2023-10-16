#include "objectbuilderfactory.h"
#include "objectbuilder.h"
const QString ab::model::ObjectBuilderFactory::ALTERATOR_SECTION_NAME = "X-Alterator Entry";
const QString ab::model::ObjectBuilderFactory::DESKTOP_SECTION_NAME   = "Desktop Entry";

namespace ab
{
namespace model
{
std::unique_ptr<ObjectBuilderInterface> ObjectBuilderFactory::getBuilder(DesktopFileParser *parser)
{
    auto sections = parser->getSections();

    auto alteratorSection = sections.find(ALTERATOR_SECTION_NAME);

    if (alteratorSection != sections.end())
    {
        //New object, return new object builder
        return std::unique_ptr<ObjectBuilderInterface>();
    }

    auto desktopSection = sections.find(DESKTOP_SECTION_NAME);

    if (desktopSection == sections.end())
    {
        //No desktop section in old object. Error? returning null-pointer
        return std::unique_ptr<ObjectBuilderInterface>();
    }

    //This is old object, return old object builder
    return std::unique_ptr<ObjectBuilderInterface>(new ObjectBuilder());
}

} // namespace model
} // namespace ab
