#ifndef CATEGORY_H
#define CATEGORY_H

#include <QMap>

#include "abstractentity.h"
#include "entitydefinitions.h"

namespace ab
{
namespace model
{
class Category : public AbstractEntity
{
public:
    void setLocale(QString locale) override;

    Object *toObject() override;
    Category *toCategory() override;
    Interface *toInterface() override;
    Application *toAplication() override;

    QString m_id                  {};
    QString m_name                {};
    QString m_comment             {};
    QString m_icon                {};
    QString m_type                {};
    QString m_xAlteratorCategory  {};

    QMap<QString, QString> m_nameLocaleStorage     {};
    QMap<QString, QString> m_commentLocaleStorage  {};
};
} // namespace model
} // namespace ab

#endif // CATEGORY_H
