#include "acobjectbuilder.h"
#include "acobjectcategorybuilder.h"

#include <QDBusReply>
#include <QDebug>

const QString DESKTOP_ENTRY_SECTION_NAME     = "Desktop Entry";
const QString ALT_CENTER_SECTION_NAME        = "X-Alterator";
const QString NAME_KEY_NAME                  = "name";
const QString CATEGORY_KEY_NAME              = "categories";
const QString TYPE_KEY_NAME                  = "type";
const QString TERMINAL_KEY_NAME              = "terminal";
const QString ICON_KEY_NAME                  = "icon";
const QString X_ALTERATOR_URI_NAME           = "x-alterator-uri";
const QString X_ALTERATOR_WEIGHT_NAME        = "x-alterator-weight";
const QString X_ALTERATOR_HELP_NAME          = "x-alterator-help";
const QString X_ALTERATOR_UI_NAME            = "x-alterator-ui";
const QString ALT_CENTER_INTERFACES_KEY_NAME = "interface";

ACObjectBuilder::ACObjectBuilder(DesktopFileParser *infoParser,
                                 QDBusInterface *categoryIface,
                                 QString getCategoryMethodName)
    : m_infoParser(infoParser)
    , m_dbusInterface(categoryIface)
    , m_dbusMethodName(getCategoryMethodName)
{}

std::unique_ptr<ACObject> ACObjectBuilder::buildACObject()
{
    std::unique_ptr<ACObject> newObject{new ACObject()};

    auto sections = m_infoParser->getSections();

    auto desktopSection = sections.find(DESKTOP_ENTRY_SECTION_NAME);

    if (desktopSection == sections.end())
    {
        qWarning() << "Can't find " << DESKTOP_ENTRY_SECTION_NAME << " section for the object! Skipping..";
        return std::unique_ptr<ACObject>(nullptr);
    }

    QString currentObjectCategoryName = getValue(*desktopSection, CATEGORY_KEY_NAME);

    setCategory(currentObjectCategoryName, m_dbusInterface, m_dbusMethodName, newObject.get());

    if (!buildNames(*desktopSection, newObject.get()))
    {
        return std::unique_ptr<ACObject>(nullptr);
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

    auto altCenterSection = sections.find(ALT_CENTER_SECTION_NAME);

    if (altCenterSection == sections.end())
    {
        qWarning() << "Can't find " << ALT_CENTER_SECTION_NAME << " section for the object! Skipping..";
    }
    else
    {
        QString interfaces = getValue(*altCenterSection, ALT_CENTER_INTERFACES_KEY_NAME);
        if (interfaces.isEmpty())
        {
            qWarning() << "Can't find interfaces for the object: " << newObject->m_id;
        }
        else
        {
            newObject->m_interfaces = parseValuesFromKey(*altCenterSection, ALT_CENTER_INTERFACES_KEY_NAME, ";");
        }
    }

    newObject->m_isLegacy = true;

    return newObject;
}

bool ACObjectBuilder::buildNames(DesktopFileParser::Section &section, ACObject *object)
{
    auto nameIt = section.find(NAME_KEY_NAME);

    if (nameIt == section.end())
    {
        qWarning() << "Can't find names for the ACObject!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(NAME_KEY_NAME);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "Can't default name for the object!";
        return false;
    }

    object->m_id = defaultName;

    object->m_displayName = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        object->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

void ACObjectBuilder::setCategory(QString categoryName, QDBusInterface *iface, QString dbusMethod, ACObject *acObject)
{
    if (categoryName.isEmpty())
    {
        qWarning() << "Can't find category name for the object: " << acObject->m_id << " using misc";
        categoryName = "Miscellaneous";
    }
    QDBusReply<QByteArray> reply = iface->call(dbusMethod, categoryName);

    if (!reply.isValid())
    {
        qWarning() << "Can't reply with category name for the object: " << acObject->m_id;

        setDefaultCategory(acObject);

        return;
    }

    QString categoryData(reply.value());

    DesktopFileParser categoryParser(categoryData);

    ACObjectCategoryBuilder categoryBuilder(&categoryParser);

    std::unique_ptr<ACObjectCategory> category = categoryBuilder.buildACObjectCategory();

    if (!category)
    {
        return;
    }

    acObject->m_categoryId = category->m_id;

    acObject->m_displayCategory = category->m_id;

    acObject->m_categoryObject = std::move(category);
}

void ACObjectBuilder::setDefaultCategory(ACObject *object)
{
    std::unique_ptr<ACObjectCategory> defaultCategory(new ACObjectCategory);

    defaultCategory->m_id = "Unknown";

    defaultCategory->m_name = "Unknown";

    defaultCategory->m_comment = "Unable to get category";

    defaultCategory->m_icon = "groups/system";

    defaultCategory->m_type = "Directory";

    defaultCategory->m_xAlteratorCategory = "X-Alterator-Unknown";

    defaultCategory->m_nameLocaleStorage["ru_RU"] = "Без категории";

    defaultCategory->m_commentLocaleStorage["ru_RU"] = "Ошибка при получении категории";
}

QString ACObjectBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString ACObjectBuilder::getValue(DesktopFileParser::Section &section, QString key)
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

std::vector<QString> ACObjectBuilder::parseValuesFromKey(DesktopFileParser::Section &section,
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
