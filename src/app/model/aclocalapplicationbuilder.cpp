#include "aclocalapplicationbuilder.h"

#include <QDebug>

const QString LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME = "Desktop Entry";
const QString LOCAL_APP_NAME_KEY_NAME              = "name";
const QString LOCAL_APP_GENERIC_NAME_KEY_NAME      = "genericname";
const QString LOCAL_APP_COMMENT_KEY_NAME           = "comment";
const QString LOCAL_APP_TRY_EXEC_KEY_NAME          = "tryexec";
const QString LOCAL_APP_ICON_KEY_NAME              = "icon";
const QString LOCAL_APP_TYPE_KEY_NAME              = "type";
const QString LOCAL_APP_CATEGORIES_KEY_NAME        = "categories";
const QString LOCAL_APP_KEYWORDS_KEY_NAME          = "keywords";
const QString LOCAL_APP_INTERFACE_KEY_NAME         = "interface";
const QString LOCAL_APP_EXEC_KEY_NAME              = "exec";
const QString LOCAL_APP_MIMETYPE_KEY_NAME          = "mimetype";
const QString LOCAL_APP_ALTCENTER_SECTION_NAME     = "X-Alterator";

std::unique_ptr<ACLocalApplication> ACLocalApplicationBuilder::buildACLocalApplicationObject(DesktopFileParser &parser)
{
    std::unique_ptr<ACLocalApplication> result(new ACLocalApplication);

    auto sections = parser.getSections();

    auto desktopEntrySectionIt = sections.find(LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME);
    if (desktopEntrySectionIt == sections.end())
    {
        qWarning() << "WARNING! Can't find " << LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME
                   << " section to buils ACLocalApplicationObject! Skipping..";
        return std::unique_ptr<ACLocalApplication>(nullptr);
    }

    auto altCenterEntrySectionIt = sections.find(LOCAL_APP_ALTCENTER_SECTION_NAME);
    if (desktopEntrySectionIt == sections.end())
    {
        qWarning() << "WARNING! Can't find " << LOCAL_APP_ALTCENTER_SECTION_NAME
                   << " section to buils ACLocalApplicationObject! Skipping..";
        return std::unique_ptr<ACLocalApplication>(nullptr);
    }

    DesktopFileParser::Section desktopEntrySection = *desktopEntrySectionIt;

    DesktopFileParser::Section altCenterEntrySection = *altCenterEntrySectionIt;

    if (!buildNames(desktopEntrySection, result.get())) //TODO BY buildFieldWithLocale
    {
        return std::unique_ptr<ACLocalApplication>();
    }

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_GENERIC_NAME_KEY_NAME,
                              result->m_genericName,
                              result->m_genericNameLocaleStorage))
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_GENERIC_NAME_KEY_NAME;
    }

    result->m_displayName = result->m_id;

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_COMMENT_KEY_NAME,
                              result->m_displayComment,
                              result->m_commentLocaleStorage))
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_COMMENT_KEY_NAME;
    }

    QString tryExec = getValue(desktopEntrySection, LOCAL_APP_TRY_EXEC_KEY_NAME);
    if (tryExec.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_TRY_EXEC_KEY_NAME;
    }
    result->m_try_Exec = tryExec;

    QString exec = getValue(desktopEntrySection, LOCAL_APP_EXEC_KEY_NAME);
    if (exec.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_EXEC_KEY_NAME;
        return std::unique_ptr<ACLocalApplication>(nullptr);
    }
    result->m_exec = exec;

    QString icon = getValue(desktopEntrySection, LOCAL_APP_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_ICON_KEY_NAME;
    }
    result->m_icon = icon;

    QString type = getValue(desktopEntrySection, LOCAL_APP_TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_TYPE_KEY_NAME;
    }
    result->m_type = type;

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_KEYWORDS_KEY_NAME,
                              result->m_displayKeywords,
                              result->m_keywordsLocaleStorage))
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_KEYWORDS_KEY_NAME;
    }

    QString interface = getValue(altCenterEntrySection, LOCAL_APP_INTERFACE_KEY_NAME);
    if (interface.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << LOCAL_APP_INTERFACE_KEY_NAME;
    }
    result->m_implementedInterface = interface;

    result->m_categories = parseValuesFromKey(desktopEntrySection, LOCAL_APP_CATEGORIES_KEY_NAME, ";");

    result->m_mimeTypes = parseValuesFromKey(desktopEntrySection, LOCAL_APP_MIMETYPE_KEY_NAME, ";");

    return result;
}

bool ACLocalApplicationBuilder::buildNames(DesktopFileParser::Section &section, ACLocalApplication *localAppObject)
{
    auto nameIt = section.find(LOCAL_APP_NAME_KEY_NAME);

    if (nameIt == section.end())
    {
        qWarning() << "WARNING! Can't find names for the local application object!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(LOCAL_APP_NAME_KEY_NAME);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "WARNING! Can't default name for the local application object!";
        return false;
    }

    localAppObject->m_id = defaultName;

    localAppObject->m_name = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        localAppObject->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool ACLocalApplicationBuilder::buildFieldWithLocale(DesktopFileParser::Section &section,
                                                     QString entryName,
                                                     QString &field,
                                                     QMap<QString, QString> &localeStorage)
{
    auto nameIt = section.find(entryName);

    if (nameIt == section.end())
    {
        qWarning() << "WARNING! Can't find entry:" << entryName;
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(entryName);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "WARNING! Can't default name for the local application object!";
        return false;
    }

    field = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        localeStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

QString ACLocalApplicationBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
{
    for (IniFileKey &currentIniFileKey : iniFileKey)
    {
        if (currentIniFileKey.keyLocale.isEmpty())
        {
            return currentIniFileKey.value.toString();
        }
    }

    return QString();
}

QString ACLocalApplicationBuilder::getValue(DesktopFileParser::Section &section, QString key)
{
    auto it = section.find(key);

    if (it == section.end())
    {
        return QString();
    }

    QList<IniFileKey> listOfKeys = section.values(key);

    if (!listOfKeys.empty())
    {
        return listOfKeys.at(0).value.toString();
    }

    return QString();
}

std::vector<QString> ACLocalApplicationBuilder::parseValuesFromKey(DesktopFileParser::Section &section,
                                                                   QString key,
                                                                   QString delimiter)
{
    QString values = getValue(section, key);
    if (values.isEmpty())
    {
        qWarning() << "WARNING! Can't find key:" << key;
        return std::vector<QString>();
    }

    if (values.back() == delimiter)
    {
        values.truncate(values.size() - 1);
    }

    QStringList valuesList = values.split(delimiter);

    std::vector<QString> result;

    for (QString &currentValue : valuesList)
    {
        if (!currentValue.isEmpty())
        {
            result.push_back(currentValue);
        }
    }

    return result;
}
