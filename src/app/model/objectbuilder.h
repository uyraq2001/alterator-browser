#ifndef AB_OBJECT_BUILDER_H
#define AB_OBJECT_BUILDER_H

#include "desktopfileparser.h"
#include "object.h"

#include <memory>
#include <QDBusInterface>

class ObjectBuilder
{
public:
    ObjectBuilder(DesktopFileParser *infoParser, QDBusInterface *categoryIface, QString getCategoryMethodName);

    std::unique_ptr<Object> buildACObject();

    bool buildNames(DesktopFileParser::Section &section, Object *object);

private:
    void setCategory(QString categoryName, QDBusInterface *iface, QString dbusMethod, Object *acObject);

    void setDefaultCategory(Object *object);

    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

private:
    DesktopFileParser *m_infoParser;
    QDBusInterface *m_dbusInterface;
    QString m_dbusMethodName;

private:
    ObjectBuilder(const ObjectBuilder &) = delete;
    ObjectBuilder(ObjectBuilder &&)      = delete;
    ObjectBuilder &operator=(const ObjectBuilder &) = delete;
    ObjectBuilder &operator=(ObjectBuilder &&) = delete;
};

#endif // AB_OBJECT_BUILDER_H
