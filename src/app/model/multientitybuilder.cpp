#include "multientitybuilder.h"
#include "constants.h"
#include "objectbuilder.h"

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
MultiEntityBuilder::MultiEntityBuilder(DesktopFileParser *infoParser)
    : m_infoParser(infoParser)
{}

std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> MultiEntityBuilder::buildAll()
{
    using namespace xalterator_entry;

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> result;

    auto sections          = m_infoParser->getSections();
    auto xalteratorSection = sections.find(XALTERATOR_SECTION);

    std::vector<QString> objectNames      = getValue(*xalteratorSection, OBJECTS_LIST_KEY).split(";");
    std::vector<QString> categoryNames    = getValue(*xalteratorSection, CATEGORIES_LIST_KEY).split(";");
    std::vector<QString> applicationNames = getValue(*xalteratorSection, APPLICATIONS_LIST_KEY).split(";");

    for (auto objectName : objectNames)
    {
        QString sectionName = OBJECT_SECTION_PREXIX << objectName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the object '" << objectName
                       << "' ! Skipping..";
            continue;
        }

        result.push_back(buildObject(currentSection));
    }

    for (auto categoryName : categoryNames)
    {
        QString sectionName = CATEGORY_SECTION_PREXIX << categoryName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the category '" << categoryName
                       << "'! Skipping..";
            continue;
        }

        result.push_back(buildCategory(currentSection));
    }

    for (auto applicationName : applicationNames)
    {
        QString sectionName = APPLICATION_SECTION_PREXIX << categoryName;
        auto currentSection = sections.find(sectionName);
        if (currentSection == sections.end())
        {
            qWarning() << "Couldn't find" << sectionName << "section for the application '" << applicationName
                       << "'! Skipping..";
            continue;
        }

        result.push_back(buildApplication(currentSection));
    }
}

std::unique_ptr<Object> MultiEntityBuilder::buildObject(DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<Object>();

    if (!buildNames(*section, res.get()))
    {
        return nullptr;
    }

    res->m_categoryId = getValue(section, CATEGORY_KEY);
    if (!buildNames(section, res.get()))
    {
        continue;
    }

    QString icon = getValue(section, ICON_KEY);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the object: " << res->m_id;
    }
    res->m_icon = icon;

    res->m_isLegacy = false;
}
std::unique_ptr<Category> MultiEntityBuilder::buildCategory(DesktopFileParser::Section section)
{
    using namespace xalterator_entry;

    auto res = std::make_unique<Category>();

    if (!buildNames(*section, res.get()))
    {
        return nullptr;
    }

    res->m_categoryId = getValue(section, CATEGORY_KEY);
    if (!buildNames(section, res.get()))
    {
        continue;
    }

    QString icon = getValue(section, ICON_KEY);
    if (icon.isEmpty())
    {
        qWarning() << "Can't find icon for the object: " << res->m_id;
    }
    res->m_icon = icon;

    res->m_isLegacy = false;
}
std::unique_ptr<LocalApplication> MultiEntityBuilder::buildApplication(DesktopFileParser::Section section) {}

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

    std::visit(
        [&defaultName, &listOfKeys](auto &&e) {
            e->m_id          = defaultName;
            e->m_displayName = defaultName;
            for (IniFileKey &currentIniFileKey : listOfKeys)
            {
                e->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
            }
        },
        entity);

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
