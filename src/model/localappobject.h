#ifndef AB_LOCAL_APPLICATION_H
#define AB_LOCAL_APPLICATION_H

#include <vector>
#include <QMap>
#include <QString>

#include "basicobject.h"

namespace ab
{
namespace model
{
class LocalAppObject : public BasicObject
{
public:
    LocalAppObject();
    virtual ~LocalAppObject();

    virtual void setLocale(QString locale) override;

public:
    QString m_genericName{};
    QString m_displayComment{};
    QString m_exec{};
    QString m_try_Exec{};
    QString m_desktopExec{};
    QString m_displayKeywords{};
    bool isTerminal{false};

    std::vector<QString> m_mimeTypes{};
    QMap<QString, QMultiMap<QString, QString>> m_alteratorIterfaces{}; // section Alterator Entry<Interface,<key, value>>

    QMap<QString, QString> m_genericNameLocaleStorage{};
    QMap<QString, QString> m_keywordsLocaleStorage{};
    QMap<QString, QString> m_commentLocaleStorage{};
};
} // namespace model
} // namespace ab

#endif // AB_LOCAL_APPLICATION_H
