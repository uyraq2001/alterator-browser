#include "localapplicationobjectbuilder.h"
#include "constants.h"

#include <memory>

namespace ao_builder
{
std::unique_ptr<ao_builder::Object> LocalApplicationObjectBuilder::buildObject(ObjectParserInterface *parser)
{
    auto result = std::make_unique<LocalAppObject>();

    LocalAppObject *object = result.get();

    if (!parseDesktopEntrySection(parser, object) || !parseAlteratorEntrySection(parser, object))
    {
        return {};
    }

    return result;
}

bool LocalApplicationObjectBuilder::parseDesktopEntrySection(ObjectParserInterface *parser,
                                                             LocalAppObject *localApplication)
{
    QString buildingFromSection = ALTERATOR_ENTRY_SECTION_NAME;

    if (!buildNames(parser, ALTERATOR_ENTRY_SECTION_NAME, localApplication))
    {
        return false;
    }

    buildFieldWithLocale(parser,
                         buildingFromSection,
                         LOCAL_APP_GENERIC_NAME_KEY_NAME,
                         localApplication->m_genericName,
                         localApplication->m_genericNameLocaleStorage);

    localApplication->m_displayName = localApplication->m_id;

    buildFieldWithLocale(parser,
                         buildingFromSection,
                         LOCAL_APP_COMMENT_KEY_NAME,
                         localApplication->m_displayComment,
                         localApplication->m_commentLocaleStorage);

    QString tryExec              = parser->getValue(buildingFromSection, LOCAL_APP_TRY_EXEC_KEY_NAME);
    localApplication->m_try_Exec = tryExec;

    QString exec = parser->getValue(buildingFromSection, LOCAL_APP_EXEC_KEY_NAME);
    if (exec.isEmpty())
    {
        return false;
    }
    localApplication->m_desktopExec = exec;

    QString icon             = parser->getValue(buildingFromSection, LOCAL_APP_ICON_KEY_NAME);
    localApplication->m_icon = icon;

    QString type = parser->getValue(buildingFromSection, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        return false;
    }
    localApplication->m_type = type;

    buildFieldWithLocale(parser,
                         buildingFromSection,
                         LOCAL_APP_KEYWORDS_KEY_NAME,
                         localApplication->m_displayKeywords,
                         localApplication->m_keywordsLocaleStorage);

    localApplication->m_mimeTypes = parseValuesFromKey(parser, buildingFromSection, LOCAL_APP_MIMETYPE_KEY_NAME, ";");

    return true;
}

bool LocalApplicationObjectBuilder::parseAlteratorEntrySection(ObjectParserInterface *parser,
                                                               LocalAppObject *localApplication)
{
    QString type = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, ALTERATOR_ENTRY_TYPE_KEY_NAME);
    if (type.isEmpty() || type != LOCAL_APP_ALTERATOR_ENTRY_SECTION_TYPE_VALUE)
    {
        return false;
    }

    QString name = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, LOCAL_APP_NAME_KEY_NAME);
    if (name.isEmpty())
    {
        return false;
    }

    QString exec = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, LOCAL_APP_ALTERATOR_ENTRYEXEC_KEY_NAME);
    if (exec.isEmpty())
    {
        return false;
    }

    QString appInterfaces = parser->getValue(ALTERATOR_ENTRY_SECTION_NAME, LOCAL_APP_ALTERATOR_INTERFACE_KEY_NAME);
    if (appInterfaces.isEmpty())
    {
        return false;
    }

    localApplication->m_type = type;
    localApplication->m_id   = name;
    localApplication->m_exec = exec;
    localApplication->m_interfaces.push_back(appInterfaces);

    return true;
}
} // namespace ao_builder
