#include "diag1objectbuilder.h"
#include "constants.h"

#include <objects/diag1object.h>

namespace ao_builder
{
std::unique_ptr<ao_builder::Object> ao_builder::Diag1ObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto result = std::make_unique<Diag1Object>();

    QString buildingFromSection = ALTERATOR_ENTRY_SECTION_NAME;

    if (!buildBase(parser, buildingFromSection, result.get()))
    {
        return nullptr;
    }

    result->m_displayName = result->m_id;

    QString type = parser->getValue(buildingFromSection, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    if (type.isEmpty() || type != KEY_TYPE_VALUE_FOR_DIAG1_OBJECT)
    {
        return nullptr;
    }
    result->m_type = type;

    if (!buildFieldWithLocale(parser,
                              buildingFromSection,
                              DIAG1_OBJECT_COMMENT_KEY_NAME,
                              result->m_comment,
                              result->m_commentLocaleStorage))
    {
        return nullptr;
    }

    QString icon = parser->getValue(buildingFromSection, DIAG1_OBJECT_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        return nullptr;
    }
    result->m_icon = icon;

    return result;
}
} // namespace ao_builder
