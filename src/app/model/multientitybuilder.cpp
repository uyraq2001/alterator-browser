#include "multientitybuilder.h"
#include "objectbuilder.h"

namespace ab
{
namespace model
{
MultiEntityBuilder::MultiEntityBuilder(DesktopFileParser *infoParser)
    : m_infoParser(infoParser)
{}

std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> buildAll()
{
    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> result;

    auto sections = m_infoParser->getSections();

    auto desktopSection    = sections.find(DESKTOP_ENTRY_SECTION_NAME);
    auto xalteratorSection = sections.find(xalterator_entry::ENTRY_SECTION);

    if (xalteratorSection == sections.end() && desktopSection == sections.end())
    {
        qWarning() << "Couldn't find nor" << DESKTOP_ENTRY_SECTION_NAME << "nor" << xalterator_entry::ENTRY_SECTION
                   << "section for the object! Skipping..";
        return nullptr;
    }

    if (xalteratorSection == sections.end())
    {
        ObjectBuilder objectBuilder(&infoParsingResult);

        std::unique_ptr<std::variant<Object, Category, LocalApplication>> newObject
            = std::make_unique<std::variant<Object, Category>>(*(objectBuilder.buildObject()));

        if (newObject)
        {
            result.push_back(std::move(newObject));
        }
    }
    else
    {
        using namespace xalterator_entry;

        std::vector<QString> objectNames      = getValue(*xalteratorSection, OBJECTS_LIST_KEY).split(";");
        std::vector<QString> categoryNames    = getValue(*xalteratorSection, CATEGORIES_LIST_KEY).split(";");
        std::vector<QString> applicationNames = getValue(*xalteratorSection, APPLICATIONS_LIST_KEY).split(";");

        for (auto objectName : objectNames)
        {
            QString sectionName = OBJECT_SECTION_PREXIX << objectName;
            auto currentSection = sections.find(sectionName);
            if (currentSection == sections.end())
            {
                qWarning() << "Couldn't find" << sectionName << "section for the object! Skipping..";
                continue;
            }

            NewObjectBuilder builder(currentSection);
            result.push_back(builder.buildObject());
        }
    }
}
} // namespace model
} // namespace ab
