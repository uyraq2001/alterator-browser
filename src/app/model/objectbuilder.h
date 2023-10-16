#ifndef AB_OBJECT_BUILDER_H
#define AB_OBJECT_BUILDER_H

#include "desktopfileparser.h"
#include "object.h"
#include "objectbuilderinterface.h"

#include <memory>
#include <QDBusInterface>

namespace ab
{
namespace model
{
class ObjectBuilder : public ObjectBuilderInterface
{
public:
    ~ObjectBuilder() = default;

    std::unique_ptr<Object> buildObject(DesktopFileParser *infoParser) override;

private:
    bool buildNames(DesktopFileParser::Section &section, Object *object);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_BUILDER_H
