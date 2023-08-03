#ifndef ACCATEGORY_H
#define ACCATEGORY_H

#include <QMap>

#include "definiciones.h"
#include "abstractacentity.h"

class ACCategory : public AbstractACEntity
{
public:
    ACCategory();
    ~ACCategory();

    void setLocale(QString locale) override;

    ACObject *toObject() override;
    ACCategory *toCategory() override;
    ACInterface *toInterface() override;
    ACApplication *toAplication() override;

    QString m_id;
    QString m_name;
    QString m_comment;
    QString m_icon;
    QString m_type;
    QString m_xAlteratorCategory;

    QMap<QString, QString> m_nameLocaleStorage;
    QMap<QString, QString> m_commentLocaleStorage;
};

#endif // ACCATEGORY_H
