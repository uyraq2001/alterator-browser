#ifndef ACOBJECT_H
#define ACOBJECT_H

#include "abstractacentity.h"
#include "definiciones.h"

#include <memory>
#include <QObject>
#include <qmap.h>

class ACObject : public AbstractACEntity
{
    Q_OBJECT
public:
    ACObject();
    ~ACObject();

    void setLocale(QString locale) override;

    ACObject *toObject() override;
    ACCategory *toCategory() override;
    ACInterface *toInterface() override;
    ACApplication *toAplication() override;

public:
    QString m_id;
    QString m_displayName;
    QString m_displayCategory;
    QString m_type;
    QString m_categoryId;
    bool m_terminal;
    QString m_icon;
    QString m_x_Alterator_URI;
    QString m_x_Alterator_Weight;
    QString m_x_Alterator_Help;
    QString m_x_Alterator_UI;

    QMap<QString, QString> m_nameLocaleStorage;

    bool m_isLegacy;

};

#endif // ACOBJECT_H
