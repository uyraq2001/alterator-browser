#ifndef AB_MODEL_DESKTOP_FILE_PARSER_H
#define AB_MODEL_DESKTOP_FILE_PARSER_H

#include <unordered_map>

#include <QString>
#include <QVariant>

namespace ab
{
namespace model
{
struct IniFileKey
{
    QString keyName{};
    QString keyLocale{};
    QVariant value{};
};

class DesktopFileParser
{
public:
    using Section  = QMultiMap<QString, IniFileKey>;
    using Sections = QMap<QString, Section>;

public:
    DesktopFileParser(QString data, QString origin = {});

public:
    QString getKeyLocale(QString keyName);
    QString getKeyNameWithoutLocale(QString keyName);
    Sections getSections();

    QString m_origin{};

private:
    Sections m_sections{};
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_DESKTOP_FILE_PARSER_H
