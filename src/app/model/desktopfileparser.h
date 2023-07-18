#ifndef ADTDESKTOPFILEPARSER_H
#define ADTDESKTOPFILEPARSER_H

#include <unordered_map>
#include <QString>
#include <QVariant>

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
    DesktopFileParser(QString data);

    QString getKeyLocale(QString keyName);

    QString getKeyNameWithoutLocale(QString keyName);

    Sections getSections();

private:
    Sections m_sections;
};

#endif // ADTDESKTOPFILEPARSER_H
