#ifndef AB_MODEL_CATEGORY_BUILDER_H
#define AB_MODEL_CATEGORY_BUILDER_H

#include "category.h"
#include "desktopfileparser.h"

#include <memory>

namespace ab
{
namespace model
{
class ObjectCategoryBuilder
{
public:
    ObjectCategoryBuilder(DesktopFileParser *categoryParser);
    ~ObjectCategoryBuilder() = default;

    std::unique_ptr<Category> buildObjectCategory();

public:
    ObjectCategoryBuilder(const ObjectCategoryBuilder &) = delete;
    ObjectCategoryBuilder(ObjectCategoryBuilder &&)      = delete;
    ObjectCategoryBuilder &operator=(const ObjectCategoryBuilder &) = delete;
    ObjectCategoryBuilder &operator=(ObjectCategoryBuilder &&) = delete;

private:
    bool buildNames(DesktopFileParser::Section &section, Category *categoryObject);
    bool buildComments(DesktopFileParser::Section &section, Category *categoryObject);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);
    QString getValue(DesktopFileParser::Section &section, QString key);

private:
    DesktopFileParser *m_categoryParser = nullptr;
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_CATEGORY_BUILDER_H
