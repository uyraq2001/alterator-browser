#include <QRegularExpression>

#include "object.h"

namespace ao_builder
{
void Object::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
        return;
    }

    QRegularExpression regex(locale + "_[A-Z]{2}");
    for (auto &fullLocale : m_nameLocaleStorage.keys())
    {
        QRegularExpressionMatch match = regex.match(fullLocale);
        if (match.hasMatch())
        {
            m_displayName = m_nameLocaleStorage[fullLocale];
        }
    }
}
} // namespace ao_builder
