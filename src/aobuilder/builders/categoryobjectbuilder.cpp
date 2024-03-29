#include "categoryobjectbuilder.h"
#include "constants.h"

#include <objects/category.h>

namespace ao_builder
{
std::unique_ptr<Object> CategoryObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto result = std::make_unique<Category>();

    QString buildingFromSection = ALTERATOR_ENTRY_SECTION_NAME;

    if (!buildBase(parser, buildingFromSection, result.get()))
    {
        return nullptr;
    }

    QString id = parser->getValue(buildingFromSection, DESKTOP_ENTRY_CATEGORY_KEY_ID);
    if (!id.isEmpty())
    {
        result->m_id = id;
    }

    QString type = parser->getValue(buildingFromSection, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    if (type.isEmpty() || type != CATEGORY_TYPE_KEY_VALUE)
    {
        return nullptr;
    }
    result->m_type = type;

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
