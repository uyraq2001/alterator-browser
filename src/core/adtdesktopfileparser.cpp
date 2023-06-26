#include "adtdesktopfileparser.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <QDebug>

const QString ADTDesktopFileParser::NAME_SECTION_NAME        = "name";
const QString ADTDesktopFileParser::ICON_SECTION_NAME        = "icon";
const QString ADTDesktopFileParser::DESCRIPTION_SECTION_NAME = "description";
const QString ADTDesktopFileParser::ARGS_SECTION_NAME        = "args";

ADTDesktopFileParser::ADTDesktopFileParser(QString data)
    : m_sections()
    , m_testLists()
    , m_dbusServiceName()
    , m_dbusPath()
    , m_dbusInterfaceName()
    , m_dbusMethodName()
{
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

                QString kcurrentSections = section.first.c_str();
                QString keyName          = QString(key.first.c_str());
                QString keyValue         = key.second.get_value("").c_str();

                currentSections.insert(getKeyNameWithoutLocale(QString(key.first.c_str())),
                                       IniFileKey{getKeyNameWithoutLocale(QString(key.first.c_str())),
                                                  getKeyLocale(QString(key.first.c_str())),
                                                  QVariant(key.second.get_value("").c_str())});
            }
        }
    }
    catch (std::exception &e)
    {
        qWarning() << "ERROR: can't parse desktop file: " << data << "\n";
    }
}

ADTDesktopFileParser::ADTDesktopFileParser(QString data,
                                           QStringList testLists,
                                           QString dbusServiceName,
                                           QString dbusPath,
                                           QString dbusInterfaceName,
                                           QString dbusMethodName)
    : ADTDesktopFileParser{data}
{
    m_testLists         = testLists;
    m_dbusServiceName   = dbusServiceName;
    m_dbusPath          = dbusPath;
    m_dbusInterfaceName = dbusInterfaceName;
    m_dbusMethodName    = dbusMethodName;
}

std::unique_ptr<ADTExecutable> ADTDesktopFileParser::buildCategoryExecutable()
{
    std::unique_ptr<ADTExecutable> newADTExecutable = std::make_unique<ADTExecutable>();

    QList<QString> groups = getGroupsList();

    if (groups.isEmpty() || groups.at(0).isEmpty())
    {
        return nullptr;
    }

    newADTExecutable->m_id = getToolName();

    newADTExecutable->m_toolId = newADTExecutable->m_id;

    QList<QString> listOfKeys = getKeysListOfGroup(newADTExecutable->m_id);

    if (listOfKeys.empty())
    {
        qWarning() << "ERROR! Can't get list of keys for: " << newADTExecutable->m_id;

        return nullptr;
    }

    setIcon(newADTExecutable->m_id, newADTExecutable.get());

    setNames(newADTExecutable->m_id, newADTExecutable.get());

    setDescriptions(newADTExecutable->m_id, newADTExecutable.get());

    setArgs(newADTExecutable->m_id, newADTExecutable.get());

    newADTExecutable->m_type = ADTExecutable::ExecutableType::categoryType;

    return newADTExecutable;
}

std::unique_ptr<ADTExecutable> ADTDesktopFileParser::buildTestExecutable(QString test, ADTExecutable *categoryExecutable)
{
    std::unique_ptr<ADTExecutable> result = std::make_unique<ADTExecutable>();

    if (!categoryExecutable)
    {
        return nullptr;
    }

    result->m_id        = test.trimmed();
    result->m_type      = ADTExecutable::ExecutableType::executableType;
    result->m_name      = test.trimmed();
    result->m_toolId    = categoryExecutable->m_id;
    result->m_icon      = categoryExecutable->m_icon;
    result->m_exit_code = categoryExecutable->m_exit_code;

    result->m_dbusServiceName   = m_dbusServiceName;
    result->m_dbusInterfaceName = m_dbusInterfaceName;
    result->m_dbusPath          = m_dbusPath;
    result->m_dbusRunMethodName = m_dbusMethodName;
    result->m_args              = result->m_id;

    auto testSection = m_sections.find(test);

    if (testSection == m_sections.end())
    {
        //No section for test found
        return nullptr;
    }

    if (!setNames(test, result.get()))
    {
        qWarning() << "ERROR! Can't names for test : " << test << " in tool: " << categoryExecutable->m_id;

        return nullptr;
    }
    if (!setDescriptions(test, result.get()))
    {
        qWarning() << "ERROR! Can't descriptions for test : " << test << " in tool: " << categoryExecutable->m_id;

        return nullptr;
    }

    if (!setArgs(test, result.get()))
    {}

    return result;
}

std::vector<std::unique_ptr<ADTExecutable>> ADTDesktopFileParser::buildExecutables()
{
    std::vector<std::unique_ptr<ADTExecutable>> results;

    std::unique_ptr<ADTExecutable> categoryExecutable = buildCategoryExecutable();

    if (!categoryExecutable.get())
    {
        return results;
    }

    ADTExecutable *currentCategory = categoryExecutable.get();

    results.push_back(std::move(categoryExecutable));

    for (QString currentTest : m_testLists)
    {
        std::unique_ptr<ADTExecutable> test = buildTestExecutable(currentTest, currentCategory);

        if (test.get())
        {
            results.push_back(std::move(test));
        }
    }

    return results;
}

QList<QString> ADTDesktopFileParser::getGroupsList() const
{
    return m_sections.keys();
}

QList<QString> ADTDesktopFileParser::getKeysListOfGroup(QString group)
{
    return m_sections[group].keys();
}

QString ADTDesktopFileParser::getKeyLocale(QString keyName)
{
    auto indexOfOpeningBracket = keyName.lastIndexOf("[");
    auto indexOfClosingBracket = keyName.lastIndexOf("]");

    if (indexOfOpeningBracket >= indexOfClosingBracket || indexOfOpeningBracket == -1 || indexOfClosingBracket == -1)
    {
        return QString();
    }

    return keyName.mid(indexOfOpeningBracket + 1, indexOfClosingBracket - indexOfOpeningBracket - 1);
}

QString ADTDesktopFileParser::getKeyNameWithoutLocale(QString keyName)
{
    auto indexOfOpeningBracket = keyName.lastIndexOf("[");
    auto indexOfClosingBracket = keyName.lastIndexOf("]");

    if (indexOfOpeningBracket >= indexOfClosingBracket || indexOfOpeningBracket == -1 || indexOfClosingBracket == -1)
    {
        return keyName;
    }

    return keyName.mid(0, indexOfOpeningBracket);
}

bool ADTDesktopFileParser::setIcon(QString &test, ADTExecutable *object)
{
    Section section = m_sections[test];
    auto iconIt     = section.find(ADTDesktopFileParser::ICON_SECTION_NAME);

    if (iconIt == section.end())
    {
        object->m_icon = DEFAULT_ICON;
    }
    else
    {
        object->m_icon = iconIt->value.toString();
    }

    return true;
}

bool ADTDesktopFileParser::setNames(QString &test, ADTExecutable *object)
{
    Section section = m_sections[test];
    auto nameIt     = section.find(ADTDesktopFileParser::NAME_SECTION_NAME);

    if (nameIt == section.end())
    {
        object->m_name = "";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(ADTDesktopFileParser::NAME_SECTION_NAME);

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        if (currentIniFileKey.keyLocale.isEmpty())
        {
            object->m_name = currentIniFileKey.value.toString();
        }
        object->m_nameLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool ADTDesktopFileParser::setDescriptions(QString &test, ADTExecutable *object)
{
    Section section = m_sections[test];
    auto nameIt     = section.find(ADTDesktopFileParser::DESCRIPTION_SECTION_NAME);

    if (nameIt == section.end())
    {
        object->m_description = "";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(ADTDesktopFileParser::DESCRIPTION_SECTION_NAME);

    for (IniFileKey &currentIniFileKey : listOfKeys)
    {
        if (currentIniFileKey.keyLocale.isEmpty())
        {
            object->m_description = currentIniFileKey.value.toString();
        }
        object->m_descriptionLocaleStorage.insert(currentIniFileKey.keyLocale, currentIniFileKey.value.toString());
    }

    return true;
}

bool ADTDesktopFileParser::setArgs(QString &test, ADTExecutable *object)
{
    Section section = m_sections[test];
    auto nameIt     = section.find(ADTDesktopFileParser::ARGS_SECTION_NAME);

    if (nameIt == section.end())
    {
        object->m_description = "";
        return false;
    }

    QList<IniFileKey> listOfKeys = section.values(ADTDesktopFileParser::ARGS_SECTION_NAME);

    if (!listOfKeys.empty())
    {
        object->m_args = listOfKeys.at(0).value.toString();
    }

    return true;
}

QString ADTDesktopFileParser::getToolName()
{
    for (QString currentSection : m_sections.keys())
    {
        bool found = false;

        for (QString currentTestName : m_testLists)
        {
            if (currentTestName == currentSection)
            {
                found = true;
            }
        }

        if (!found)
        {
            return currentSection;
        }
    }
    qWarning() << "ERROR! Can't find tool name!";

    return QString();
}
