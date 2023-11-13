#ifndef AB_CATEGORY_H
#define AB_CATEGORY_H

#include <QMap>

#include "object.h"

namespace ab
{
namespace model
{
class Category : public Object
{
public:
    Category();
    virtual ~Category();

public:
    QString m_comment{};
    QString m_xAlteratorCategory{};

    QMap<QString, QString> m_commentLocaleStorage{};

    void setLocale(QString locale);
};
} // namespace model
} // namespace ab

#endif // AB_CATEGORY_H
