#ifndef AB_MODEL_CATEGORY_H
#define AB_MODEL_CATEGORY_H

#include <QMap>

namespace ab
{
namespace model
{
class Category
{
public:
    void setLocale(QString locale);

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

#endif // AB_MODEL_CATEGORY_H
