#ifndef AB_OBJECT_CATEGORY_H
#define AB_OBJECT_CATEGORY_H

#include <QMap>
#include <QObject>

namespace ab
{
namespace model
{
class ObjectCategory
{
public:
    void setLocale(QString locale);

public:
    QString m_id{};
    QString m_name{};
    QString m_comment{};
    QString m_icon{};
    QString m_type{};
    QString m_xAlteratorCategory{};

    QMap<QString, QString> m_nameLocaleStorage{};
    QMap<QString, QString> m_commentLocaleStorage{};
};
} // namespace model
} // namespace ab

#endif // AB_OBJECT_CATEGORY_H
