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
    MultiEntityBuilder()  = default;
    ~MultiEntityBuilder() = default;

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> buildAll(
        DesktopFileParser *infoParser) override;

    MultiEntityBuilder(const MultiEntityBuilder &) = delete;
    MultiEntityBuilder(MultiEntityBuilder &&)      = delete;
    MultiEntityBuilder &operator=(const MultiEntityBuilder &) = delete;
    MultiEntityBuilder &operator=(MultiEntityBuilder &&) = delete;

private:
    std::unique_ptr<std::variant<Object, Category, LocalApplication>> buildObject(DesktopFileParser::Section section);
    std::unique_ptr<std::variant<Object, Category, LocalApplication>> buildCategory(DesktopFileParser::Section section);
    std::unique_ptr<std::variant<Object, Category, LocalApplication>> buildApplication(
        DesktopFileParser::Section section);

    bool buildNames(DesktopFileParser::Section &section, std::variant<Object, Category, LocalApplication> *entity);
    QString getDefaultValue(QList<IniFileKey> iniFileKey);
    QString getValue(DesktopFileParser::Section &section, QString key);
    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);
};
} // namespace model
} // namespace ab

#endif // MULTIENTITYBUILDER_H
