#ifndef ACOBJECTCATEGORY_H
#define ACOBJECTCATEGORY_H

#include <QMap>
#include <QObject>

class ACObjectCategory
{
public:
    ACObjectCategory();
    ~ACObjectCategory();

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

#endif // ACOBJECTCATEGORY_H
