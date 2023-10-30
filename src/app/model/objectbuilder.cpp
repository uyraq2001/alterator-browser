#include "objectbuilder.h"
#include "desktopfileparser.h"

#include <QDBusReply>
#include <QDebug>

const QString ab::model::ObjectBuilder::DESKTOP_ENTRY_SECTION_NAME = "Desktop Entry";
const QString ab::model::ObjectBuilder::NAME_KEY_NAME              = "name";
const QString ab::model::ObjectBuilder::CATEGORY_KEY_NAME          = "categories";
const QString ab::model::ObjectBuilder::TYPE_KEY_NAME              = "type";
const QString ab::model::ObjectBuilder::TERMINAL_KEY_NAME          = "terminal";
const QString ab::model::ObjectBuilder::ICON_KEY_NAME              = "icon";
const QString ab::model::ObjectBuilder::X_ALTERATOR_URI_NAME       = "x-alterator-uri";
const QString ab::model::ObjectBuilder::X_ALTERATOR_WEIGHT_NAME    = "x-alterator-weight";
const QString ab::model::ObjectBuilder::X_ALTERATOR_HELP_NAME      = "x-alterator-help";
const QString ab::model::ObjectBuilder::X_ALTERATOR_UI_NAME        = "x-alterator-ui";
const QString ab::model::ObjectBuilder::X_ALTERATOR_INTERNAL_NAME  = "x-alterator-internal-name";

namespace ab
{
namespace model
{
std::unique_ptr<Object> ObjectBuilder::buildObject(DesktopFileParser *infoParser)
{
    std::unique_ptr<Object> newObject{new Object()};

    auto sections = infoParser->getSections();

    auto desktopSection = sections.find(DESKTOP_ENTRY_SECTION_NAME);

    if (desktopSection == sections.end())
    {
        qWarning() << "Can't find " << DESKTOP_ENTRY_SECTION_NAME << " section for the object! Skipping..";
        return nullptr;
    }

    QString currentObjectCategoryName = getValue(*desktopSection, CATEGORY_KEY_NAME);

    newObject->m_categoryId = currentObjectCategoryName;

    if (!buildNames(*desktopSection, newObject.get()))
    {
        return nullptr;
    }

    QString type = getValue(*desktopSection, TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        qWarning() << "Can't find type for the object: " << newObject->m_id;
    }
    newObject->m_type = type;

    QString icon = getValue(*desktopSection, ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the object: " << newObject->m_id;
    }
    newObject->m_icon = icon;

    QString x_Alterator_URI = getValue(*desktopSection, X_ALTERATOR_URI_NAME);
    if (x_Alterator_URI.isEmpty())
    {
        qWarning() << "Can't find x_Alterator_URI for the object: " << newObject->m_id;
    }
    newObject->m_x_Alterator_URI = x_Alterator_URI;

    QString x_Alterator_Weight = getValue(*desktopSection, X_ALTERATOR_WEIGHT_NAME);
    if (x_Alterator_Weight.isEmpty())
    {
        qWarning() << "Can't find x_Alterator_Weight for the object: " << newObject->m_id;
    }
    newObject->m_x_Alterator_Weight = x_Alterator_Weight;

    QString x_Alterator_Help = getValue(*desktopSection, X_ALTERATOR_HELP_NAME);
    if (x_Alterator_Help.isEmpty())
    {
        qWarning() << "Can't find x_Alterator_Help for the object: " << newObject->m_id;
    }
    newObject->m_x_Alterator_Help = x_Alterator_Help;

    QString x_Alterator_UI = getValue(*desktopSection, X_ALTERATOR_UI_NAME);
    if (x_Alterator_UI.isEmpty())
    {
        qWarning() << "Can't find x_Alterator_UI for the object: " << newObject->m_id;
    }
    newObject->m_x_Alterator_UI = x_Alterator_UI;

    QString terminal = getValue(*desktopSection, TERMINAL_KEY_NAME);
    if (terminal.isEmpty())
    {
        qWarning() << "Can't find terminal for the object: " << newObject->m_id;
    }
    if (terminal.toLower() == QString("true"))
    {
        newObject->m_terminal = true;
    }

    newObject->m_isLegacy = true;

    return newObject;
}

bool ObjectBuilder::buildNames(DesktopFileParser::Section &section, Object *object)
{
    auto nameIt = section.find(NAME_KEY_NAME);

    if (nameIt == section.end())
    {
        qWarning() << "Can't find names for the Object";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(NAME_KEY_NAME);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "Can't default name for the object";
        return false;
    }

    QString x_Alterator_Internal_Name = section.values(X_ALTERATOR_INTERNAL_NAME)[0].keyName;
    if (x_Alterator_Internal_Name.isEmpty())
    {
        qWarning() << "Can't find x_Alterator_Internal_Name for the object";
    }
    object->m_id = x_Alterator_Internal_Name;

    object->m_displayName = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        object->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

QString ObjectBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
{
    for (IniFileKey &currentIniFileKey : iniFileKey)
    {
        if (currentIniFileKey.keyLocale.isEmpty())
        {
            return currentIniFileKey.value.toString();
        }
    }

    return {};
}

QString ObjectBuilder::getValue(DesktopFileParser::Section &section, QString key)
{
    auto it = section.find(key);
    if (it == section.end())
    {
        return {};
    }

    QList<IniFileKey> listOfKeys = section.values(key);

    if (!listOfKeys.empty())
    {
        return listOfKeys.at(0).value.toString();
    }

    return {};
}

std::vector<QString> ObjectBuilder::parseValuesFromKey(DesktopFileParser::Section &section,
                                                       QString key,
                                                       QString delimiter)
{
    QString values = getValue(section, key);
    if (values.isEmpty())
    {
        qWarning() << "Can't find key:" << key;
        return {};
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
