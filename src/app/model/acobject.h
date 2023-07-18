#ifndef ACOBJECT_H
#define ACOBJECT_H

#include "acobjectcategory.h"

#include <memory>
#include <QObject>
#include <qmap.h>

class ACObject : public QObject
{
    Q_OBJECT
public:
    ACObject();
    ~ACObject();

    void setLocale(QString locale);

public:
    QString m_id;
    QString m_displayName;
    QString m_displayCategory;
    QString m_type;
    bool m_terminal;
    QString m_icon;
    QString m_x_Alterator_URI;
    QString m_x_Alterator_Weight;
    QString m_x_Alterator_Help;
    QString m_x_Alterator_UI;

    std::vector<QString> m_interfaces;

    QMap<QString, QString> m_nameLocaleStorage;

    std::unique_ptr<ACObjectCategory> m_categoryObject;
};

#endif // ACOBJECT_H
