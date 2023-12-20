#include "baseobjectbuilder.h"
#include "constants.h"
#include "parsers/objectparserinterface.h"

namespace ao_builder
{
bool BaseObjectBuilder::buildFieldWithLocale(ObjectParserInterface *parser,
                                             QString sectionName,
                                             QString entryName,
                                             QString &field,
                                             QMap<QString, QString> &localeStorage)
{
    auto sections = parser->getSections();

    auto alteratorEntrySectionIt = sections.find(sectionName);
    if (alteratorEntrySectionIt == sections.end())
    {
        return false;
    }

    auto section = *alteratorEntrySectionIt;

    auto nameIt = section.find(entryName);

    if (nameIt == section.end())
    {
        return false;
    }

    QList<ObjectParserInterface::IniField> listOfKeys = section.values(entryName);

    QString defaultName = parser->getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        return false;
    }

    field = defaultName;

    for (ObjectParserInterface::IniField &currentIniFileKey : listOfKeys)
    {
        localeStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool BaseObjectBuilder::buildNames(ObjectParserInterface *parser, QString sectionName, Object *localAppObject)
{
    const auto sections = parser->getSections();

    const auto alteratorEntrySectionIt = sections.find(sectionName);
    if (alteratorEntrySectionIt == sections.end())
    {
        return false;
    }

    ObjectParserInterface::Section section = *alteratorEntrySectionIt;

    const auto nameIt = section.find(ALTERATOR_ENTRY_OBJECT_KEY_NAME);
    if (nameIt == section.end())
    {
        return false;
    }

    const auto listOfKeys = section.values(ALTERATOR_ENTRY_OBJECT_KEY_NAME);

    const QString defaultName = parser->getDefaultValue(listOfKeys);

    if (defaultName.isEmpty())
    {
        return false;
    }

    localAppObject->m_id = defaultName;

    for (const ObjectParserInterface::IniField &currentIniFileKey : listOfKeys)
    {
        localAppObject->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale.split('_').first(),
                                                   currentIniFileKey.value.toString());
    }

    return true;
}

std::vector<QString> BaseObjectBuilder::parseValuesFromKey(ObjectParserInterface *parser,
                                                           QString section,
                                                           QString key,
                                                           QString delimiter)
{
    QString values = parser->getValue(section, key);
    if (values.isEmpty())
    {
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

} // namespace ao_builder
