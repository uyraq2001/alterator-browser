#include "objectbuilderfactory.h"
#include "categoryobjectbuilder.h"
#include "constants.h"
#include "legacyobjectbuilder.h"
#include "localapplicationobjectbuilder.h"
#include <memory>

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

    if (desktopSection != sections.end())
    {
        // Old object, return old object builder
        return getLegacyObjectBuilder(parser);
    }

    // No Desktop Section and no Alterator Entry. Error
    return {};
}

std::unique_ptr<ObjectBuilderInterface> ObjectBuilderFactory::getObjectBuilder(ObjectParserInterface *parser)
{
    QString type = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);

    if (type == KEY_TYPE_VALUE_FOR_APPLICATION_OBJECT)
    {
        return std::make_unique<LocalApplicationObjectBuilder>();
    }
    else if (type == KEY_TYPE_VALUE_FOR_CATEGORY_OBJECT)
    {
        return std::make_unique<CategoryObjectBuilder>();
    }
    else if (type == KEY_TYPE_VALUE_FOR_OBJECT)
    {
        // TODO: builder for new object is not implemented
        return {};
    }

    // Unknown type
    return {};
}

std::unique_ptr<ObjectBuilderInterface> ObjectBuilderFactory::getLegacyObjectBuilder(ObjectParserInterface *parser)
{
    QString type = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, DESKTOP_ENTRY_TYPE_KEY_NAME);

    if (type == LEGACY_KEY_TYPE_VALUE_FOR_OBJECT)
    {
        return std::make_unique<LegacyObjectBuilder>();
    }

    // Unknown type
    return {};
}

} // namespace ao_builder
