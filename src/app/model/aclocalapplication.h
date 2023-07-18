#ifndef ACLOCALAPPLICATION_H
#define ACLOCALAPPLICATION_H

#include <vector>
#include <QObject>
#include <qmap.h>

class ACLocalApplication : public QObject
{
    Q_OBJECT
public:
    ACLocalApplication();
    ~ACLocalApplication();

    void setLocale(QString locale);

public:
    QString m_id;
    QString m_name;
    QString m_genericName;
    QString m_comment;
    QString m_type;
    QString m_icon;
    QString m_exec;
    QString m_termminal;
    std::vector<QString> m_categories;
    std::vector<QString> m_mimeType;
    std::vector<QString> m_currentKeywords;
    bool m_startupNotify;

    QMap<QString, std::vector<QString>> m_keywords;
    QMap<QString, QString> m_nameLocaleStorage;
    QMap<QString, QString> m_genericNameLocaleStorage;
    QMap<QString, QString> m_commentLocaleStorage;
};

#endif // ACLOCALAPPLICATION_H
