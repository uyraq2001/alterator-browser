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
    static const QString DESKTOP_ENTRY_SECTION_NAME;
    static const QString NAME_KEY_NAME;
    static const QString CATEGORY_KEY_NAME;
    static const QString TYPE_KEY_NAME;
    static const QString TERMINAL_KEY_NAME;
    static const QString ICON_KEY_NAME;
    static const QString X_ALTERATOR_URI_NAME;
    static const QString X_ALTERATOR_WEIGHT_NAME;
    static const QString X_ALTERATOR_HELP_NAME;
    static const QString X_ALTERATOR_UI_NAME;

public:
    ~ObjectBuilder() = default;

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> buildAll(
        DesktopFileParser *infoParser) override;

private:
    bool buildNames(DesktopFileParser::Section &section, Object *object);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_BUILDER_H
