#include "objectbuilder.h"
#include "constants.h"

#include <memory>

namespace ao_builder
{
std::unique_ptr<Object> ObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto newObject = std::make_unique<Object>();

    if (!buildNames(parser, ALTERATOR_ENTRY_SECTION_NAME, newObject.get()))
    {
        return nullptr;
    }

    if (!buildFieldWithLocale(parser,
                              ALTERATOR_ENTRY_SECTION_NAME,
                              OBJECT_DISPLAY_NAME_KEY_NAME,
                              newObject->m_displayName,
                              newObject->m_nameLocaleStorage))
    {
        newObject->m_displayName = newObject->m_id;
    }

    QString currentObjectCategoryName = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, OBJECT_CATEGORY_KEY_NAME);
    newObject->m_categoryId           = currentObjectCategoryName;

    QString type      = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    newObject->m_type = type;

    QString icon      = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, OBJECT_ICON_KEY_NAME);
    newObject->m_icon = icon;

    newObject->m_isLegacy = false;

    return newObject;
}
} // namespace ao_builder
