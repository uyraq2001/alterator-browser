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
    ObjectBuilder(DesktopFileParser *infoParser, QDBusInterface *categoryIface, QString getCategoryMethodName);
    ~ObjectBuilder() = default;

public:
    ObjectBuilder(const ObjectBuilder &) = delete;
    ObjectBuilder(ObjectBuilder &&)      = delete;
    ObjectBuilder &operator=(const ObjectBuilder &) = delete;
    ObjectBuilder &operator=(ObjectBuilder &&) = delete;

    std::unique_ptr<Object> buildObject();
    bool buildNames(DesktopFileParser::Section &section, Object *object);

private:
    void setCategory(QString categoryName, QDBusInterface *iface, QString dbusMethod, Object *acObject);
    void setDefaultCategory(Object *object);
    QString getDefaultValue(QList<IniFileKey> iniFileKey);
    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

private:
    DesktopFileParser *m_infoParser = nullptr;
    QDBusInterface *m_dbusInterface = nullptr;
    QString m_dbusMethodName{};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_BUILDER_H
