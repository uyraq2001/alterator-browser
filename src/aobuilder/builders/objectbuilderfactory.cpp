
#include "objectbuilderfactory.h"
#include "categoryobjectbuilder.h"
#include "constants.h"
#include "legacyobjectbuilder.h"
#include "localapplicationobjectbuilder.h"

namespace ao_builder
{
std::unique_ptr<ObjectBuilderInterface> ObjectBuilderFactory::getBuilder(ObjectParserInterface *parser)
{
    auto sections = parser->getSections();

    auto alteratorSection = sections.find(ALTERATOR_ENTRY_SECTION_NAME);

    if (alteratorSection != sections.end())
    {
        // New object, return new object builder
        return getObjectBuilder(parser);
    }

    auto desktopSection = sections.find(DESKTOP_ENTRY_SECTION_NAME);

    if (desktopSection == sections.end())
    {
        // No desktop section in old object. Error? returning null-pointer
        return std::unique_ptr<ObjectBuilderInterface>();
    }

    // This is old object, return old object builder
    return std::unique_ptr<ObjectBuilderInterface>(new LegacyObjectBuilder());

    return nullptr;
}

std::unique_ptr<ObjectBuilderInterface> ObjectBuilderFactory::getObjectBuilder(ObjectParserInterface *parser)
{
    QString type = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);

    if (type.isEmpty())
    {
        return nullptr;
    }

    if (type == KEY_TYPE_VALUE_FOR_APPLICATION_OBJECT)
    {
        return std::unique_ptr<ObjectBuilderInterface>(new LocalApplicationObjectBuilder());
    }
    else if (type == KEY_TYPE_VALUE_FOR_CATEGORY_OBJECT)
    {
        return std::unique_ptr<ObjectBuilderInterface>(new CategoryObjectBuilder());
    }
    else if (type == KEY_TYPE_VALUE_FOR_OBJECT)
    {
        return nullptr;
    }

    return nullptr;
}

} // namespace ao_builder
