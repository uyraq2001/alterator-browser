#ifndef AOB_OBJECT_H
#define AOB_OBJECT_H

#include <QMap>
#include <QMetaType>
#include <QString>

#include <optional>

const int DEFAULT_WEIGHT = 500;

namespace ao_builder
{
using Id = QString;

class Object
{
public:
    Object()          = default;
    virtual ~Object() = default;

    virtual void setLocale(QString locale);

protected:
    QString findLocale(QString locale, QMap<QString, QString> localeStorage);

public:
    Id m_id{};
    QString m_displayName{};
    QString m_type{};
    QString m_icon{};
    int m_weight{DEFAULT_WEIGHT}; // NOTE(mchernigin): supposed to be [0, 1000]

    QString m_dbus_path{};
    QString m_interface{};

    QString m_categoryId{};
    bool m_isLegacy{false};

    QMap<QString, QString> m_nameLocaleStorage{};
};
} // namespace ao_builder

Q_DECLARE_METATYPE(ao_builder::Object *)

#endif // AOB_OBJECT_H
