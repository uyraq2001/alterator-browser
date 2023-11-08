#ifndef AB_MODEL_LOCAL_APPLICATION_BUILDER_H
#define AB_MODEL_LOCAL_APPLICATION_BUILDER_H

#include "desktopfileparser.h"
#include "localapplication.h"

#include <memory>

namespace ab
{
namespace model
{
class LocalApplicationBuilder // TODO(kozyrevid): redisign this model completely to make apps into underlings of interfaces (which could be implemented simply as strings)
{
public:
    LocalApplicationBuilder() = default;

    std::unique_ptr<LocalApplication> buildLocalApplicationObject(DesktopFileParser &parser);

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
} // namespace model
} // namespace ab

#endif // AB_MODEL_LOCAL_APPLICATION_BUILDER_H
