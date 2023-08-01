#ifndef ACAPPLICATION_H
#define ACAPPLICATION_H

#include "abstractacentity.h"
#include "definiciones.h"

class ACApplication: public AbstractACEntity
{
public:
    ACApplication();
    ~ACApplication();

    void setLocale(QString locale) override;

    ACObject *toObject() override;
    ACCategory *toCategory() override;
    ACInterface *toInterface() override;
    ACApplication *toAplication() override;

    QString m_id;
    QString m_name;
    QString m_genericName;
    QString m_displayName;
    QString m_displayComment;
    QString m_try_Exec;
    QString m_exec;
    QString m_icon;
    QString m_type;
    QString m_displayKeywords;
    QString m_implementedInterface;

    std::vector<QString> m_categories;
    std::vector<QString> m_mimeTypes;
    QMultiMap<QString, QMap<QString, QString>> m_alteratorIterfaces; // section X-Alterator Entry<Interface,<key, value>>

    QMap<QString, QString> m_nameLocaleStorage;
    QMap<QString, QString> m_genericNameLocaleStorage;
    QMap<QString, QString> m_keywordsLocaleStorage;
    QMap<QString, QString> m_commentLocaleStorage;
};

#endif // ACAPPLICATION_H
