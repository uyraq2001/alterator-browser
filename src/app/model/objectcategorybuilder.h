#ifndef ACOBJECTCATEGORYBUILDER_H
#define ACOBJECTCATEGORYBUILDER_H

#include "objectcategory.h"
#include "desktopfileparser.h"

#include <memory>

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

#endif // ACOBJECTCATEGORYBUILDER_H
