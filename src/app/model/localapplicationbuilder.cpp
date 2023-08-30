#include "localapplicationbuilder.h"

#include <QDebug>

namespace ab
{
namespace model
{
const QString LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME         = "Desktop Entry";
const QString LOCAL_APP_NAME_KEY_NAME                      = "name";
const QString LOCAL_APP_GENERIC_NAME_KEY_NAME              = "genericname";
const QString LOCAL_APP_COMMENT_KEY_NAME                   = "comment";
const QString LOCAL_APP_TRY_EXEC_KEY_NAME                  = "tryexec";
const QString LOCAL_APP_ICON_KEY_NAME                      = "icon";
const QString LOCAL_APP_TYPE_KEY_NAME                      = "type";
const QString LOCAL_APP_CATEGORIES_KEY_NAME                = "categories";
const QString LOCAL_APP_KEYWORDS_KEY_NAME                  = "keywords";
const QString LOCAL_APP_INTERFACE_KEY_NAME                 = "implements";
const QString LOCAL_APP_EXEC_KEY_NAME                      = "exec";
const QString LOCAL_APP_MIMETYPE_KEY_NAME                  = "mimetype";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_NAME     = "Alterator Entry";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_PREFIX   = "Alterator Interface";
const QString LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_KEY_NAME = "exec";

std::unique_ptr<LocalApplication> LocalApplicationBuilder::buildACLocalApplicationObject(DesktopFileParser &parser)
{
    std::unique_ptr<LocalApplication> result(new LocalApplication);

    auto sections = parser.getSections();

    auto desktopEntrySectionIt = sections.find(LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME);
    if (desktopEntrySectionIt == sections.end())
    {
        qWarning() << "Can't find " << LOCAL_APP_DESKTOP_ENTRY_SECTION_NAME
                   << " section to buils ACLocalApplicationObject! Skipping..";
        return std::unique_ptr<LocalApplication>(nullptr);
    }

    DesktopFileParser::Section desktopEntrySection = *desktopEntrySectionIt;

    if (!buildNames(desktopEntrySection, result.get())) //TODO BY buildFieldWithLocale
    {
        return std::unique_ptr<LocalApplication>();
    }

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_GENERIC_NAME_KEY_NAME,
                              result->m_genericName,
                              result->m_genericNameLocaleStorage))
    {
        qWarning() << "Can't find key:" << LOCAL_APP_GENERIC_NAME_KEY_NAME;
    }

    result->m_displayName = result->m_id;

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_COMMENT_KEY_NAME,
                              result->m_displayComment,
                              result->m_commentLocaleStorage))
    {
        qWarning() << "Can't find key:" << LOCAL_APP_COMMENT_KEY_NAME;
    }

    QString tryExec = getValue(desktopEntrySection, LOCAL_APP_TRY_EXEC_KEY_NAME);
    if (tryExec.isEmpty())
    {
        qWarning() << "Can't find key:" << LOCAL_APP_TRY_EXEC_KEY_NAME;
    }
    result->m_try_Exec = tryExec;

    QString exec = getValue(desktopEntrySection, LOCAL_APP_EXEC_KEY_NAME);
    if (exec.isEmpty())
    {
        qWarning() << "Can't find key:" << LOCAL_APP_EXEC_KEY_NAME;
        return std::unique_ptr<LocalApplication>(nullptr);
    }
    result->m_desktopExec = exec;

    QString icon = getValue(desktopEntrySection, LOCAL_APP_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find key:" << LOCAL_APP_ICON_KEY_NAME;
    }
    result->m_icon = icon;

    QString type = getValue(desktopEntrySection, LOCAL_APP_TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        qWarning() << "Can't find key:" << LOCAL_APP_TYPE_KEY_NAME;
    }
    result->m_type = type;

    if (!buildFieldWithLocale(desktopEntrySection,
                              LOCAL_APP_KEYWORDS_KEY_NAME,
                              result->m_displayKeywords,
                              result->m_keywordsLocaleStorage))
    {
        qWarning() << "Can't find key:" << LOCAL_APP_KEYWORDS_KEY_NAME;
    }

    bool xAlteratorEntrySectionParsingResult = parseXAlteratorEntrySection(parser, result.get());

    result->m_categories = parseValuesFromKey(desktopEntrySection, LOCAL_APP_CATEGORIES_KEY_NAME, ";");

    result->m_mimeTypes = parseValuesFromKey(desktopEntrySection, LOCAL_APP_MIMETYPE_KEY_NAME, ";");

    return result;
}

bool LocalApplicationBuilder::parseXAlteratorEntrySection(DesktopFileParser &parser, LocalApplication *localApplication)
{
    auto sections = parser.getSections();

    auto xAlteratorEntrySectionIt = sections.find(LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_NAME);

    if (xAlteratorEntrySectionIt == sections.end())
    {
        qWarning() << "Can't find " << LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_NAME
                   << " section to buils ACLocalApplicationObject! Skipping..";
        return false;
    }

    DesktopFileParser::Section xAlteratorEntrySection = *xAlteratorEntrySectionIt;

    QString interfaces = getValue(xAlteratorEntrySection, LOCAL_APP_INTERFACE_KEY_NAME);

    QStringList interfacesList = interfaces.split(";", Qt::SkipEmptyParts);

    if (interfacesList.isEmpty())
    {
        qWarning() << "Can't find interfaces list in section " << LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_NAME
                   << " section to buils ACLocalApplicationObject! Skipping..";
        return false;
    }

    for (QString &currentInterface : interfacesList)
    {
        QString currentSectionName(LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_PREFIX + currentInterface);

        auto currentSectionIt = sections.find(currentSectionName);

        if (currentSectionIt == sections.end())
        {
            qWarning() << "Can't find section X_Alterator section" << currentSectionName
                       << " section to buils ACLocalApplicationObject! Skipping..";

            return false;
        }

        QString execValue = getValue(*currentSectionIt, LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_KEY_NAME);

        if (execValue.isEmpty())
        {
            qWarning() << "Can't find key " << LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_KEY_NAME
                       << " in section: " << currentSectionName << "Skipping..";

            return false;
        }

        QMap<QString, QString> execMap;
        execMap.insert(LOCAL_APP_X_ALTERATOR_ENTRY_SECTION_KEY_NAME, execValue);

        localApplication->m_alteratorIterfaces.insert(currentInterface, execMap);
    }

    return true;
}

bool LocalApplicationBuilder::buildNames(DesktopFileParser::Section &section, LocalApplication *localAppObject)
{
    auto nameIt = section.find(LOCAL_APP_NAME_KEY_NAME);

    if (nameIt == section.end())
    {
        qWarning() << "Can't find names for the local application object!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(LOCAL_APP_NAME_KEY_NAME);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "Can't default name for the local application object!";
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

bool LocalApplicationBuilder::buildFieldWithLocale(DesktopFileParser::Section &section,
                                                   QString entryName,
                                                   QString &field,
                                                   QMap<QString, QString> &localeStorage)
{
    auto nameIt = section.find(entryName);

    if (nameIt == section.end())
    {
        qWarning() << "Can't find entry:" << entryName;
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(entryName);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "Can't default name for the local application object!";
        return false;
    }

    field = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        localeStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

QString LocalApplicationBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString LocalApplicationBuilder::getValue(DesktopFileParser::Section &section, QString key)
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

std::vector<QString> LocalApplicationBuilder::parseValuesFromKey(DesktopFileParser::Section &section,
                                                                 QString key,
                                                                 QString delimiter)
{
    QString values = getValue(section, key);
    if (values.isEmpty())
    {
        qWarning() << "Can't find key:" << key;
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
} // namespace model
} // namespace ab
