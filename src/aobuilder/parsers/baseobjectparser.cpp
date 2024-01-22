#include <unordered_map>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <qdebug.h>

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
                Section &currentSections = m_sections[QString::fromStdString(section.first)];

                currentSections.insert(getKeyNameWithoutLocale(QString::fromStdString(key.first)),
                                       IniField{getKeyNameWithoutLocale(QString::fromStdString(key.first)),
                                                getKeyLocale(QString::fromStdString(key.first)),
                                                QVariant(key.second.get_value("").c_str())});
            }
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "Got exception while parsing object:" << e.what();
        return false;
    }

    return true;
}

QString BaseObjectParser::getKeyLocale(QString keyName)
{
    auto indexOfOpeningBracket = keyName.lastIndexOf("[");
    auto indexOfClosingBracket = keyName.lastIndexOf("]");

    if (indexOfOpeningBracket >= indexOfClosingBracket || indexOfOpeningBracket == -1 || indexOfClosingBracket == -1)
    {
        qWarning() << QString("No locale for '%1' key").arg(keyName);
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

QString BaseObjectParser::getDefaultValue(QList<IniField> iniFileKey)
{
    for (IniField &currentIniFileKey : iniFileKey)
    {
        if (currentIniFileKey.keyLocale.isEmpty())
        {
            return currentIniFileKey.value.toString();
        }
    }

    if (!iniFileKey.isEmpty())
    {
        qWarning() << QString("No default value for key '%1'").arg(iniFileKey[0].key);
    }
    else
    {
        qWarning() << "Empty key doesn't containt default value";
    }
    return {};
}

QString BaseObjectParser::getValue(QString section, QString key)
{
    auto sectionIt = m_sections.find(section);

    if (sectionIt == m_sections.end())
    {
        qWarning() << QString("There is no section '%1' in the file").arg(section);
        return {};
    }

    auto it = (*sectionIt).find(key);

    if (it == (*sectionIt).end())
    {
        qWarning() << QString("Section '%1' doesn't contain key '%2'").arg(section, key);
        return {};
    }

    QList<IniField> listOfKeys = (*sectionIt).values(key);

    if (!listOfKeys.empty())
    {
        return listOfKeys.at(0).value.toString();
    }

    qWarning() << QString("Key '%1' is empty in section '%1'").arg(key, section);
    return {};
}
} // namespace ao_builder
