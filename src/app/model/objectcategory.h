#ifndef AB_OBJECT_CATEGORY_H
#define AB_OBJECT_CATEGORY_H

#include <QMap>
#include <QObject>

class ObjectCategory
{
public:
    ObjectCategory();
    ~ObjectCategory();

    QString m_id;
    QString m_name;
    QString m_comment;
    QString m_icon;
    QString m_type;
    QString m_xAlteratorCategory;

    QMap<QString, QString> m_nameLocaleStorage;
    QMap<QString, QString> m_commentLocaleStorage;

    void setLocale(QString locale);
};

#endif // AB_OBJECT_CATEGORY_H
