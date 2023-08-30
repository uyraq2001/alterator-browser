#ifndef ACOBJECTCATEGORYBUILDER_H
#define ACOBJECTCATEGORYBUILDER_H

#include "objectcategory.h"
#include "desktopfileparser.h"

#include <memory>

class ACObjectCategoryBuilder
{
public:
public:
    ACObjectCategoryBuilder(DesktopFileParser *categoryParser);
    ~ACObjectCategoryBuilder();

    std::unique_ptr<ACObjectCategory> buildACObjectCategory();

private:
    DesktopFileParser *m_categoryParser;

private:
    bool buildNames(DesktopFileParser::Section &section, ACObjectCategory *categoryObject);
    bool buildComments(DesktopFileParser::Section &section, ACObjectCategory *categoryObject);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

private:
    ACObjectCategoryBuilder(const ACObjectCategoryBuilder &) = delete;
    ACObjectCategoryBuilder(ACObjectCategoryBuilder &&)      = delete;
    ACObjectCategoryBuilder &operator=(const ACObjectCategoryBuilder &) = delete;
    ACObjectCategoryBuilder &operator=(ACObjectCategoryBuilder &&) = delete;
};

#endif // ACOBJECTCATEGORYBUILDER_H
