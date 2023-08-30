#ifndef ACOBJECT_H
#define ACOBJECT_H

#include "localapplication.h"
#include "objectcategory.h"

#include <memory>
#include <QObject>
#include <qmap.h>

class Object : public QObject
{
    Q_OBJECT
public:
    Object();
    ~Object();

    void setLocale(QString locale);

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

    std::vector<QString> m_interfaces;
    std::vector<LocalApplication *> m_applications;

    QMap<QString, QString> m_nameLocaleStorage;

    std::unique_ptr<ObjectCategory> m_categoryObject;

    bool m_isLegacy;
};

#endif // ACOBJECT_H
