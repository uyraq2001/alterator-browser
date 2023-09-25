#include "categorybuilder.h"

#include <QDebug>
#include <QList>
#include <QMultiMap>

namespace ab
{
namespace model
{
const QString DESKTOP_ENTRY_SECTION_NAME             = "Desktop Entry";
const QString CATEGORY_NAME_KEY_NAME                 = "name";
const QString CATEGORY_COMMENT_KEY_NAME              = "comment";
const QString CATEGORY_ICON_KEY_NAME                 = "icon";
const QString CATEGORY_TYPE_KEY_NAME                 = "type";
const QString CATEGORY_X_ALTERATOR_CATEGORY_KEY_NAME = "x-alterator-category";

ObjectCategoryBuilder::ObjectCategoryBuilder(DesktopFileParser *categoryParser)
    : m_categoryParser(categoryParser)
{}

std::unique_ptr<Category> ObjectCategoryBuilder::buildObjectCategory()
{
    auto result = std::make_unique<Category>();

    auto sections = m_categoryParser->getSections();

    auto desktopSection = sections.find(DESKTOP_ENTRY_SECTION_NAME);
    if (desktopSection == sections.end())
    {
        qWarning() << "Can't find" << DESKTOP_ENTRY_SECTION_NAME << "section! Skipping...";
        return {};
    }

    if (!buildNames(*desktopSection, result.get()))
    {
        return {};
    }

    if (!buildComments(*desktopSection, result.get()))
    {
        return {};
    }

    QString icon = getValue(*desktopSection, CATEGORY_ICON_KEY_NAME);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the category: " << result->m_id;
        return {};
    }
    result->m_icon = icon;

    QString type = getValue(*desktopSection, CATEGORY_TYPE_KEY_NAME);
    if (type.isEmpty())
    {
        qWarning() << "Can't find type for the category: " << result->m_id;
        return {};
    }
    result->m_type = type;

    QString xAlteratorCategory = getValue(*desktopSection, CATEGORY_X_ALTERATOR_CATEGORY_KEY_NAME);
    if (xAlteratorCategory.isEmpty())
    {
        qWarning() << "Can't find X-Alterator-Category for the category: " << result->m_id;
        return {};
    }
    result->m_xAlteratorCategory = xAlteratorCategory;

    return result;
}

bool ObjectCategoryBuilder::buildNames(DesktopFileParser::Section &section, Category *categoryObject)
{
    auto nameIt = section.find(CATEGORY_NAME_KEY_NAME);
    if (nameIt == section.end())
    {
        qWarning() << "Can't find names for the category!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(CATEGORY_NAME_KEY_NAME);
    QString defaultName          = getDefaultValue(listOfKeys);
    if (defaultName.isEmpty())
    {
        qWarning() << "Can't default name for the category!";
        return false;
    }

    categoryObject->m_id   = defaultName;
    categoryObject->m_name = defaultName;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        categoryObject->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool ObjectCategoryBuilder::buildComments(DesktopFileParser::Section &section, Category *categoryObject)
{
    auto commentIt = section.find(CATEGORY_COMMENT_KEY_NAME);
    if (commentIt == section.end())
    {
        qWarning() << "Can't find comments for the category!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(CATEGORY_COMMENT_KEY_NAME);
    QString defaultComment       = getDefaultValue(listOfKeys);
    if (defaultComment.isEmpty())
    {
        qWarning() << "Can't default comment for the category!";
        return false;
    }

    categoryObject->m_comment = defaultComment;

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        categoryObject->m_commentLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

QString ObjectCategoryBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString ObjectCategoryBuilder::getValue(DesktopFileParser::Section &section, QString key)
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
} // namespace model
} // namespace ab
