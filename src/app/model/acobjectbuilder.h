#ifndef ACOBJECTBUILDER_H
#define ACOBJECTBUILDER_H

#include "acobject.h"
#include "desktopfileparser.h"

#include <memory>
#include <QDBusInterface>

class ACObjectBuilder
{
public:
    ACObjectBuilder(DesktopFileParser *infoParser, QDBusInterface *categoryIface, QString getCategoryMethodName);

    std::unique_ptr<ACObject> buildACObject();

    bool buildNames(DesktopFileParser::Section &section, ACObject *object);

private:
    QString getDefaultValue(QList<IniFileKey> iniFileKey);

    QString getValue(DesktopFileParser::Section &section, QString key);

    std::vector<QString> parseValuesFromKey(DesktopFileParser::Section &section, QString key, QString delimiter);

private:
    DesktopFileParser *m_infoParser;
    QDBusInterface *m_dbusInterface;
    QString m_dbusMethodName;

private:
    ACObjectBuilder(const ACObjectBuilder &) = delete;
    ACObjectBuilder(ACObjectBuilder &&)      = delete;
    ACObjectBuilder &operator=(const ACObjectBuilder &) = delete;
    ACObjectBuilder &operator=(ACObjectBuilder &&) = delete;
};

#endif // ACOBJECTBUILDER_H
