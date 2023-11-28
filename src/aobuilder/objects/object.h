#ifndef AOB_OBJECT_H
#define AOB_OBJECT_H

#include <QMap>
#include <QString>

namespace ao_builder
{
class Object
{
public:
    Object()          = default;
    virtual ~Object() = default;

    virtual void setLocale(QString locale);

public:
    QString m_id{};
    QString m_displayName{};
    QString m_type{};
    QString m_icon{};

    QMap<QString, QString> m_nameLocaleStorage{};
};
} // namespace objectbuilder

#endif // AOB_OBJECT_H
