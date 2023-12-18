#include "newobjectbuilder.h"
#include "../objects/legacyobject.h"
#include "constants.h"

#include <memory>

namespace ao_builder
{
std::unique_ptr<Object> NewObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto newObject = std::make_unique<LegacyObject>();

    const auto buildingObject = dynamic_cast<LegacyObject *>(newObject.get());

    if (!buildNames(parser, ALTERATOR_ENTRY_SECTION_NAME, buildingObject))
    {
        return nullptr;
    }

    if (!buildFieldWithLocale(parser,
                              ALTERATOR_ENTRY_SECTION_NAME,
                              OBJECT_DISPLAY_NAME_KEY_NAME,
                              buildingObject->m_displayName,
                              buildingObject->m_nameLocaleStorage))
    {
        buildingObject->m_displayName = buildingObject->m_id;
    }

    QString currentObjectCategoryName = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, OBJECT_CATEGORY_KEY_NAME);
    buildingObject->m_categoryId      = currentObjectCategoryName;

    QString type           = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    buildingObject->m_type = type;

    QString icon           = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, OBJECT_ICON_KEY_NAME);
    buildingObject->m_icon = icon;

    buildingObject->m_isLegacy = false;

    return newObject;
}
} // namespace ao_builder
