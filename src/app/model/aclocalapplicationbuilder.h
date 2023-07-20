#ifndef ACLOCALAPPLICATIONBUILDER_H
#define ACLOCALAPPLICATIONBUILDER_H

#include "aclocalapplication.h"
#include "desktopfileparser.h"

#include <memory>

class ACLocalApplicationBuilder
{
public:
    ACLocalApplicationBuilder() = default;

    std::unique_ptr<ACLocalApplication> buildACLocalApplicationObject(DesktopFileParser &parser);

private:
    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    bool buildNames(DesktopFileParser::Section &section, ACLocalApplication *localAppObject);

    bool buildFieldWithLocale(DesktopFileParser::Section &section,
                              QString entryName,
                              QString &field,
                              QMap<QString, QString> &localeStorage);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);
};

#endif // ACLOCALAPPLICATIONBUILDER_H
