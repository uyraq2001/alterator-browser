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
std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> MultiEntityBuilder::buildAll(
    DesktopFileParser *infoParser)
{
    using namespace xalterator_entry;

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> result;

    auto sections          = infoParser->getSections();
    auto xalteratorSection = sections.find(XALTERATOR_SECTION);

    QStringList objectNames      = getValue(*xalteratorSection, OBJECTS_LIST_KEY).split(";");
    QStringList categoryNames    = getValue(*xalteratorSection, CATEGORIES_LIST_KEY).split(";");
    QStringList applicationNames = getValue(*xalteratorSection, APPLICATIONS_LIST_KEY).split(";");

    for (auto objectName : objectNames)
    {
        QString sectionName = OBJECT_SECTION_PREXIX + " " + objectName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the object '" << objectName
                       << "' ! Skipping..";
            continue;
        }

        result.push_back(buildObject(*currentSection));
    }

    for (auto categoryName : categoryNames)
    {
        QString sectionName = CATEGORY_SECTION_PREXIX + " " + categoryName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the category '" << categoryName
                       << "'! Skipping..";
            continue;
        }

        result.push_back(buildCategory(*currentSection));
    }

    for (auto applicationName : applicationNames)
    {
        QString sectionName = APPLICATION_SECTION_PREXIX + " " + applicationName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the application '" << applicationName
                       << "'! Skipping..";
            continue;
        }

        result.push_back(buildApplication(*currentSection));
    }

    return result;
}

std::unique_ptr<std::variant<Object, Category, LocalApplication>> MultiEntityBuilder::buildObject(
    DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<std::variant<Object, Category, LocalApplication>>(
        std::variant<Object, Category, LocalApplication>(Object()));

    if (!buildNames(section, res.get()))
    {
        return nullptr;
    }

    QString currentObjectCategoryName   = getValue(section, CATEGORY_KEY);
    std::get<Object>(*res).m_categoryId = currentObjectCategoryName;

    QString icon = getValue(section, ICON_KEY);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the object: " << std::get<Object>(*res).m_id;
    }
    std::get<Object>(*res).m_icon = icon;

    std::get<Object>(*res).m_isLegacy = false;

    return res;
}

std::unique_ptr<std::variant<Object, Category, LocalApplication>> MultiEntityBuilder::buildCategory(
    DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<std::variant<Object, Category, LocalApplication>>(
        std::variant<Object, Category, LocalApplication>(Category()));
    return res;
}
std::unique_ptr<std::variant<Object, Category, LocalApplication>> MultiEntityBuilder::buildApplication(
    DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<std::variant<Object, Category, LocalApplication>>(
        std::variant<Object, Category, LocalApplication>(Category()));
    return res;
}

bool MultiEntityBuilder::buildNames(DesktopFileParser::Section &section,
                                    std::variant<Object, Category, LocalApplication> *entity)
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

    std::visit(Overload{[&defaultName, &listOfKeys](Object &e) {
                            e.m_id          = defaultName;
                            e.m_displayName = defaultName;
                            for (IniFileKey &currentIniFileKey : listOfKeys)
                            {
                                e.m_nameLocaleStorage.insert(currentIniFileKey.keyLocale,
                                                             currentIniFileKey.value.toString());
                            }
                        },
                        [&defaultName, &listOfKeys](Category &e) {
                            e.m_id   = defaultName;
                            e.m_name = defaultName;
                            for (IniFileKey &currentIniFileKey : listOfKeys)
                            {
                                e.m_nameLocaleStorage.insert(currentIniFileKey.keyLocale,
                                                             currentIniFileKey.value.toString());
                            }
                        },
                        [&listOfKeys](auto &e) {
                            for (IniFileKey &currentIniFileKey : listOfKeys)
                            {}
                            // TODO: needs cleaning up
                        }},
               *entity);

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
