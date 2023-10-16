#ifndef AB_OBJECT_BUILDER_H
#define AB_OBJECT_BUILDER_H

#include "desktopfileparser.h"
#include "object.h"

#include <memory>
#include <QDBusInterface>

namespace ab
{
namespace model
{
class ObjectBuilder
{
public:
    ObjectBuilder(DesktopFileParser *infoParser);

    std::unique_ptr<Object> buildObject();

private:
    bool buildNames(DesktopFileParser::Section &section, Object *object);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

private:
    DesktopFileParser *m_infoParser = nullptr;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_BUILDER_H
