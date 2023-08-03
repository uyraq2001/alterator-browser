#include "acobjectcategorybuilder.h"

#include <QDebug>
#include <QList>
#include <QMultiMap>

const QString DESKTOP_ENTRY_SECTION_NAME             = "Desktop Entry";
const QString CATEGORY_NAME_KEY_NAME                 = "name";
const QString CATEGORY_COMMENT_KEY_NAME              = "comment";
const QString CATEGORY_ICON_KEY_NAME                 = "icon";
const QString CATEGORY_TYPE_KEY_NAME                 = "type";
const QString CATEGORY_X_ALTERATOR_CATEGORY_KEY_NAME = "x-alterator-category";

ACObjectCategoryBuilder::ACObjectCategoryBuilder(DesktopFileParser *categoryParser)
    : m_categoryParser(categoryParser)
{}

ACObjectCategoryBuilder::~ACObjectCategoryBuilder() {}

std::unique_ptr<ACCategory> ACObjectCategoryBuilder::buildACObjectCategory()
{
    std::unique_ptr<ACCategory> result{new ACCategory};

    auto sections = m_categoryParser->getSections();

    auto desktopSection = sections.find(DESKTOP_ENTRY_SECTION_NAME);

    if (desktopSection == sections.end())
    {
        qWarning() << "WARNING! Can't find " << DESKTOP_ENTRY_SECTION_NAME << " section! Skipping..";
        return std::unique_ptr<ACCategory>();
    }

    if (!buildNames(*desktopSection, result.get()))
    {
        return std::unique_ptr<ACCategory>();
    }

    if (!buildComments(*desktopSection, result.get()))
    {
        return std::unique_ptr<ACCategory>();
    }

    QString icon = getValue(*desktopSection, CATEGORY_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        qWarning() << "WARNING! Can't find icon for the category: " << result->m_id;
        return std::unique_ptr<ACCategory>();
    }
    result->m_icon = icon;

    QString type = getValue(*desktopSection, CATEGORY_TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        qWarning() << "WARNING! Can't find type for the category: " << result->m_id;
        return std::unique_ptr<ACCategory>();
    }
    result->m_type = type;

    QString xAlteratorCategory = getValue(*desktopSection, CATEGORY_X_ALTERATOR_CATEGORY_KEY_NAME);
    if (xAlteratorCategory.isEmpty())
    {
        qWarning() << "WARNING! Can't find X-Alterator-Category for the category: " << result->m_id;
        return std::unique_ptr<ACCategory>();
    }
    result->m_xAlteratorCategory = xAlteratorCategory;

    return result;
}

bool ACObjectCategoryBuilder::buildNames(DesktopFileParser::Section &section, ACCategory *categoryObject)
{
    auto nameIt = section.find(CATEGORY_NAME_KEY_NAME);

    if (nameIt == section.end())
    {
        qWarning() << "WARNING! Can't find names for the category!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(CATEGORY_NAME_KEY_NAME);

    QString defaultName = getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        qWarning() << "WARNING! Can't default name for the category!";
        return false;
    }

    categoryObject->m_id = defaultName;

    categoryObject->m_name = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        categoryObject->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool ACObjectCategoryBuilder::buildComments(DesktopFileParser::Section &section, ACCategory *categoryObject)
{
    auto commentIt = section.find(CATEGORY_COMMENT_KEY_NAME);

    if (commentIt == section.end())
    {
        qWarning() << "WARNING! Can't find comments for the category!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(CATEGORY_COMMENT_KEY_NAME);

    QString defaultComment = getDefaultValue(listOfKeys);

    if (defaultComment.isEmpty())
    {
        qWarning() << "WARNING! Can't default comment for the category!";
        return false;
    }

    categoryObject->m_comment = defaultComment;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        categoryObject->m_commentLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

QString ACObjectCategoryBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString ACObjectCategoryBuilder::getValue(DesktopFileParser::Section &section, QString key)
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
