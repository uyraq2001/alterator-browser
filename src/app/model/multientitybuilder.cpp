#include "multientitybuilder.h"
#include "constants.h"
#include "objectbuilder.h"

#include <QDebug>

template<typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

namespace ab
{
namespace model
{
std::unique_ptr<Object> MultiEntityBuilder::buildAll(DesktopFileParser *infoParser)
{
    using namespace xalterator_entry;

    std::unique_ptr<Object> result;

    auto sections          = infoParser->getSections();
    auto xalteratorSection = sections.find(XALTERATOR_SECTION);

    QStringList objectNames = getValue(*xalteratorSection, OBJECTS_LIST_KEY).split(";");

    for (auto objectName : objectNames)
    {
        QString sectionName = OBJECT_SECTION_PREXIX + objectName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the object '" << objectName
                       << "' ! Skipping..";
            continue;
        }

        result = buildObject(currentSection.value());
    }

    return result;
}

std::unique_ptr<Object> MultiEntityBuilder::buildObject(DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<Object>();

    if (!buildNames(section, res.get()))
    {
        return nullptr;
    }

    res->m_categoryId = getValue(section, CATEGORY_KEY);
    if (!buildNames(section, res.get()))
    {
        return nullptr;
    }

    QString icon = getValue(section, ICON_KEY);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the object: " << res->m_id;
    }
    res->m_icon = icon;

    res->m_isLegacy = false;

    return res;
}
//std::unique_ptr<Category> MultiEntityBuilder::buildCategory(DesktopFileParser::Section section)
//{
//    using namespace xalterator_entry;

//    auto res = std::make_unique<Category>();

//    if (!buildNames(section, res.get()))
//    {
//        return nullptr;
//    }

//    res->m_categoryId = getValue(section, CATEGORY_KEY);
//    if (!buildNames(section, res.get()))
//    {
//        continue;
//    }

//    QString icon = getValue(section, ICON_KEY);
//    if (icon.isEmpty())
//    {
//        qWarning() << "Can't find icon for the object: " << res->m_id;
//    }
//    res->m_icon = icon;

//    res->m_isLegacy = false;
//}
//std::unique_ptr<LocalApplication> MultiEntityBuilder::buildApplication(DesktopFileParser::Section section) {}

bool MultiEntityBuilder::buildNames(DesktopFileParser::Section &section, Object *object)
{
    using namespace xalterator_entry;

    auto nameIt = section.find(NAME_KEY);

    if (nameIt == section.end())
    {
        qWarning() << "Can't find names for the Object!";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(NAME_KEY);

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

QString MultiEntityBuilder::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString MultiEntityBuilder::getValue(DesktopFileParser::Section &section, QString key)
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

std::vector<QString> MultiEntityBuilder::parseValuesFromKey(DesktopFileParser::Section &section,
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
