#ifndef AOB_OBJECT_H
#define AOB_OBJECT_H

#include <QMap>
#include <QMetaType>
#include <QString>

namespace ao_builder
{
using Id = QString;

class Object
{
public:
    Object()          = default;
    virtual ~Object() = default;

    virtual void setLocale(QString locale);

public:
    Id m_id{};
    QString m_displayName{};
    QString m_type{};
    QString m_icon{};

    QMap<QString, QString> m_nameLocaleStorage{};
};
} // namespace ao_builder

Q_DECLARE_METATYPE(ao_builder::Object *)

#endif // AOB_OBJECT_H
