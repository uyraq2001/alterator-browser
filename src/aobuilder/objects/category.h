#ifndef AOB_CATEGORY_H
#define AOB_CATEGORY_H

#include <QMap>

#include "object.h"

namespace ao_builder
{
class Category : public Object
{
public:
    Category();
    virtual ~Category();

public:
    QString m_comment{};

    QMap<QString, QString> m_commentLocaleStorage{};

    void setLocale(QString locale);
};
} // namespace ao_builder

#endif // AB_CATEGORY_H
