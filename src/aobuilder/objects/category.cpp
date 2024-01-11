#include <QRegularExpression>

#include "category.h"

namespace ao_builder
{
void Category::setLocale(QString locale)
{
    Object::setLocale(locale);

    auto commentIt = m_commentLocaleStorage.find(locale);
    if (commentIt != m_commentLocaleStorage.end())
    {
        m_comment = *commentIt;
    }
    else
    {
        m_comment = findLocale(locale, m_commentLocaleStorage).value_or(m_comment);
    }
}
} // namespace ao_builder
