#include "legacycategoryobjectbuilder.h"
#include "constants.h"

#include <objects/category.h>

namespace ao_builder
{
std::unique_ptr<Object> LegacyCategoryObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto result = std::make_unique<Category>();

    QString buildingFromSection = DESKTOP_ENTRY_SECTION_NAME;

    if (!buildNames(parser, buildingFromSection, result.get()))
    {
        return nullptr;
    }

    result.get()->m_displayName = result.get()->m_id;

    QString type = parser->getValue(buildingFromSection, DESKTOP_ENTRY_TYPE_KEY_NAME);
    if (type.isEmpty() || type != LEGACY_CATEGORY_TYPE_VALUE)
    {
        return nullptr;
    }
    result->m_type = type;

    QString id = parser->getValue(buildingFromSection, DESKTOP_ENTRY_CATEGORY_KEY_ID);
    if (!id.isEmpty())
    {
        result.get()->m_id = id;
    }

    if (!buildFieldWithLocale(parser,
                              buildingFromSection,
                              CATEGORY_COMMENT_KEY_NAME,
                              result.get()->m_comment,
                              result.get()->m_commentLocaleStorage))
    {
        return nullptr;
    }

    QString icon = parser->getValue(buildingFromSection, CATEGORY_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        return nullptr;
    }
    result->m_icon = icon;

    return result;
}
} // namespace ao_builder
