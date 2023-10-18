#ifndef MULTIENTITYBUILDER_H
#define MULTIENTITYBUILDER_H

#include "category.h"
#include "desktopfileparser.h"
#include "localapplication.h"
#include "object.h"
#include "objectbuilderinterface.h"

#include <memory>
#include <variant>
#include <vector>

namespace ab
{
namespace model
{
class MultiEntityBuilder : public ObjectBuilderInterface
{
public:
    MultiEntityBuilder(DesktopFileParser *infoParser);

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> buildAll();

private:
    std::unique_ptr<Object> buildObject(DesktopFileParser::Section section);
    std::unique_ptr<Category> buildCategory(DesktopFileParser::Section section);
    std::unique_ptr<LocalApplication> buildApplication(DesktopFileParser::Section section);

    bool buildNames(DesktopFileParser::Section &section, Object *object);
    QString getDefaultValue(QList<IniFileKey> iniFileKey);
    QString getValue(DesktopFileParser::Section &section, QString key);
    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

    DesktopFileParser *m_infoParser = nullptr;
};
} // namespace model
} // namespace ab

#endif // MULTIENTITYBUILDER_H
