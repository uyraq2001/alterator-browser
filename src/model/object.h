#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include <QMap>
#include <QString>

namespace ab
{
namespace model
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
    std::vector<QString> m_interfaces{};

    QMap<QString, QString> m_nameLocaleStorage{};
    //    std::vector<LocalApplication *> m_applications{};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_H
