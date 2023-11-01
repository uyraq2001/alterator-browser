#ifndef AB_LOCAL_APPLICATION_H
#define AB_LOCAL_APPLICATION_H

#include <vector>
#include <QMap>
#include <QObject>
#include <QString>

namespace ab
{
namespace model
{
class LocalApplication
{
public:
    void setLocale(QString locale);

public:
    QString m_id{};
    QString m_name{};
    QString m_genericName{};
    QString m_displayName{};
    QString m_displayComment{};
    QString m_try_Exec{};
    QString m_desktopExec{};
    QString m_icon{};
    QString m_type{};
    QString m_displayKeywords{};
    QString m_implementedInterface{};

    std::vector<QString> m_categories{};
    std::vector<QString> m_mimeTypes{};
    QMap<QString, QMultiMap<QString, QString>> m_alteratorIterfaces{}; // section Alterator Entry<Interface,<key, value>>

    QMap<QString, QString> m_nameLocaleStorage{};
    QMap<QString, QString> m_genericNameLocaleStorage{};
    QMap<QString, QString> m_keywordsLocaleStorage{};
    QMap<QString, QString> m_commentLocaleStorage{};
};
} // namespace model
} // namespace ab

#endif // AB_LOCAL_APPLICATION_H
