#ifndef ACLOCALAPPLICATIONBUILDER_H
#define ACLOCALAPPLICATIONBUILDER_H

#include "localapplication.h"
#include "desktopfileparser.h"

#include <memory>

class LocalApplicationBuilder
{
public:
    LocalApplicationBuilder() = default;

    std::unique_ptr<LocalApplication> buildACLocalApplicationObject(DesktopFileParser &parser);

private:
    bool parseXAlteratorEntrySection(DesktopFileParser &parser, LocalApplication *localApplication);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    bool buildNames(DesktopFileParser::Section &section, LocalApplication *localAppObject);

    bool buildFieldWithLocale(DesktopFileParser::Section &section,
                              QString entryName,
                              QString &field,
                              QMap<QString, QString> &localeStorage);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);
};

#endif // ACLOCALAPPLICATIONBUILDER_H
