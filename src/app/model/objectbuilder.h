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

    bool buildNames(DesktopFileParser::Section &section, Object *object);

private:
    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

private:
    DesktopFileParser *m_infoParser;

private:
    ObjectBuilder(const ObjectBuilder &) = delete;
    ObjectBuilder(ObjectBuilder &&)      = delete;
    ObjectBuilder &operator=(const ObjectBuilder &) = delete;
    ObjectBuilder &operator=(ObjectBuilder &&) = delete;
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_BUILDER_H
