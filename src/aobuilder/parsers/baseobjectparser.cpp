#include <unordered_map>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "baseobjectparser.h"

namespace ao_builder
{
ObjectParserInterface::Sections BaseObjectParser::getSections()
{
    return m_sections;
}

bool BaseObjectParser::parse(QString data)
{
    m_sections.clear();

    std::istringstream iStream(data.toStdString());

    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(iStream, pt);

        for (auto &section : pt)
        {
            for (auto &key : section.second)
            {
                Section &currentSections = m_sections[QString(section.first.c_str())];

                currentSections.insert(getKeyNameWithoutLocale(QString(key.first.c_str())),
                                       IniFileKey{getKeyNameWithoutLocale(QString(key.first.c_str())),
                                                  getKeyLocale(QString(key.first.c_str())),
                                                  QVariant(key.second.get_value("").c_str())});
            }
        }
        return true;
    }
    catch (std::exception &e)
    {
        return false;
    }
}

QString BaseObjectParser::getKeyLocale(QString keyName)
{
    auto indexOfOpeningBracket = keyName.lastIndexOf("[");
    auto indexOfClosingBracket = keyName.lastIndexOf("]");

    if (indexOfOpeningBracket >= indexOfClosingBracket || indexOfOpeningBracket == -1 || indexOfClosingBracket == -1)
    {
        return {};
    }

    return keyName.mid(indexOfOpeningBracket + 1, indexOfClosingBracket - indexOfOpeningBracket - 1);
}

QString BaseObjectParser::getKeyNameWithoutLocale(QString keyName)
{
    auto indexOfOpeningBracket = keyName.lastIndexOf("[");
    auto indexOfClosingBracket = keyName.lastIndexOf("]");

    if (indexOfOpeningBracket >= indexOfClosingBracket || indexOfOpeningBracket == -1 || indexOfClosingBracket == -1)
    {
        return keyName;
    }

    return keyName.mid(0, indexOfOpeningBracket);
}

QString BaseObjectParser::getDefaultValue(QList<IniFileKey> iniFileKey)
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

QString BaseObjectParser::getValue(QString section, QString key)
{
    auto sectionIt = m_sections.find(section);

    if (sectionIt == m_sections.end())
    {
        return {};
    }

    auto it = (*sectionIt).find(key);

    if (it == (*sectionIt).end())
    {
        return {};
    }

    QList<IniFileKey> listOfKeys = (*sectionIt).values(key);

    if (!listOfKeys.empty())
    {
        return listOfKeys.at(0).value.toString();
    }

    return {};
}
} // namespace ao_builder
