#include "legacyobjectbuilder.h"
#include "../objects/legacyobject.h"
#include "constants.h"
#include <memory>

namespace ao_builder
{
std::unique_ptr<Object> LegacyObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto newObject = std::make_unique<LegacyObject>();

    const auto buildingObject = dynamic_cast<LegacyObject *>(newObject.get());

    if (!buildNames(parser, DESKTOP_ENTRY_SECTION_NAME, buildingObject))
    {
        return nullptr;
    }

    buildingObject->m_displayName = buildingObject->m_id;

    QString currentObjectCategoryName = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_CATEGORY_KEY_NAME);
    buildingObject->m_categoryId      = currentObjectCategoryName;

    QString type           = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    buildingObject->m_type = type;

    QString icon           = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_ICON_KEY_NAME);
    buildingObject->m_icon = icon;

    QString x_Alterator_URI = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_X_ALTERATOR_URI_KEY_NAME);
    buildingObject->m_x_Alterator_URI = x_Alterator_URI;

    QString x_Alterator_Weight = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_X_ALTERATOR_WEIGHT_KEY_NAME);
    buildingObject->m_x_Alterator_Weight = x_Alterator_Weight;

    QString x_Alterator_Help = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_X_ALTERATOR_HELP_KEY_NAME);
    buildingObject->m_x_Alterator_Help = x_Alterator_Help;

    QString x_Alterator_UI = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_X_ALTERATOR_UI_KEY_NAME);
    buildingObject->m_x_Alterator_UI = x_Alterator_UI;

    QString terminal = parser->getValue(DESKTOP_ENTRY_SECTION_NAME, LEGACY_OBJECT_TERMINAL_KEY_NAME);
    if (terminal.toLower() == QString("true"))
    {
        buildingObject->m_isTerminal = true;
    }

    QString x_Alterator_Internal_Name = parser->getValue(DESKTOP_ENTRY_SECTION_NAME,
                                                         LEGACY_OBJECT_X_ALTERATOR_INTERNAL_KEY_NAME);

    buildingObject->m_x_Alterator_Internal_Name = x_Alterator_Internal_Name;

    buildingObject->m_isLegacy = true;

    return newObject;
}
} // namespace ao_builder
