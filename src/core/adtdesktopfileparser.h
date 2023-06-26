#ifndef ADTDESKTOPFILEPARSER_H
#define ADTDESKTOPFILEPARSER_H

#include "adtexecutable.h"

#include <unordered_map>
#include <QString>
#include <QVariant>

struct IniFileKey
{
    QString keyName{};
    QString keyLocale{};
    QVariant value{};
};

class ADTDesktopFileParser
{
public:
    using Section  = QMultiMap<QString, IniFileKey>;
    using Sections = QMap<QString, Section>;

    const QString DEFAULT_ICON = "system-run";

    static const QString NAME_SECTION_NAME;
    static const QString ICON_SECTION_NAME;
    static const QString DESCRIPTION_SECTION_NAME;
    static const QString ARGS_SECTION_NAME;

public:
    ADTDesktopFileParser(QString data,
                         QStringList testLists,
                         QString dbusServiceName,
                         QString dbusPath,
                         QString dbusInterfaceName,
                         QString dbusMethodName);

    std::unique_ptr<ADTExecutable> buildCategoryExecutable();
    std::unique_ptr<ADTExecutable> buildTestExecutable(QString test, ADTExecutable *categoryExecutable);
    std::vector<std::unique_ptr<ADTExecutable>> buildExecutables();

    QList<QString> getGroupsList() const;
    QList<QString> getKeysListOfGroup(QString group);

    QString getKeyLocale(QString keyName);

private:
    ADTDesktopFileParser(QString data);

    QString getKeyNameWithoutLocale(QString keyName);
    bool setIcon(QString &test, ADTExecutable *object);
    bool setNames(QString &test, ADTExecutable *object);
    bool setDescriptions(QString &test, ADTExecutable *object);
    bool setArgs(QString &test, ADTExecutable *object);
    QString getToolName();

private:
    Sections m_sections;

    QStringList m_testLists;

    QString m_dbusServiceName;
    QString m_dbusPath;
    QString m_dbusInterfaceName;
    QString m_dbusMethodName;
};

#endif // ADTDESKTOPFILEPARSER_H
