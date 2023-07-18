#ifndef ACOBJECT_H
#define ACOBJECT_H

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
    QString m_name;
    QString m_category;
    std::vector<QString> m_interfaces;

    QMap<QString, QString> m_nameLocaleStorage;
    QMap<QString, QString> m_categoryLocaleStorage;
};

#endif // ACOBJECT_H
