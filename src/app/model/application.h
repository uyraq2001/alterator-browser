#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMap>

#include "abstractentity.h"
#include "entitydefinitions.h"

namespace ab
{
namespace model
{
class Application : public AbstractEntity
{
public:
    void setLocale(QString locale) override;

    Object *toObject() override;
    Category *toCategory() override;
    Interface *toInterface() override;
    Application *toAplication() override;

    QString m_id                   = {};
    QString m_name                 = {};
    QString m_genericName          = {};
    QString m_displayName          = {};
    QString m_displayComment       = {};
    QString m_try_Exec             = {};
    QString m_exec                 = {};
    QString m_icon                 = {};
    QString m_type                 = {};
    QString m_displayKeywords      = {};
    QString m_implementedInterface = {};

    std::vector<QString> m_categories = {};
    std::vector<QString> m_mimeTypes  = {};
    QMultiMap<QString, QMap<QString, QString>> m_alteratorIterfaces
        = {}; // section X-Alterator Entry<Interface,<key, value>>

    QMap<QString, QString> m_nameLocaleStorage        = {};
    QMap<QString, QString> m_genericNameLocaleStorage = {};
    QMap<QString, QString> m_keywordsLocaleStorage    = {};
    QMap<QString, QString> m_commentLocaleStorage     = {};
};
} // namespace model
} // namespace ab

#endif // APPLICATION_H
