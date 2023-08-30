#ifndef AB_OBJECT_CATEGORY_BUILDER_H
#define AB_OBJECT_CATEGORY_BUILDER_H

#include "desktopfileparser.h"
#include "objectcategory.h"

#include <memory>

namespace ab
{
namespace model
{
class ObjectCategoryBuilder
{
public:
public:
    ObjectCategoryBuilder(DesktopFileParser *categoryParser);
    ~ObjectCategoryBuilder();

    std::unique_ptr<ObjectCategory> buildACObjectCategory();

private:
    DesktopFileParser *m_categoryParser;

private:
    bool buildNames(DesktopFileParser::Section &section, ObjectCategory *categoryObject);
    bool buildComments(DesktopFileParser::Section &section, ObjectCategory *categoryObject);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

private:
    ObjectCategoryBuilder(const ObjectCategoryBuilder &) = delete;
    ObjectCategoryBuilder(ObjectCategoryBuilder &&)      = delete;
    ObjectCategoryBuilder &operator=(const ObjectCategoryBuilder &) = delete;
    ObjectCategoryBuilder &operator=(ObjectCategoryBuilder &&) = delete;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_CATEGORY_BUILDER_H
