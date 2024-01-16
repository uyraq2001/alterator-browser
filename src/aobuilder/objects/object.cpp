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

    m_displayName = findLocale(locale, m_nameLocaleStorage);
}

QString Object::findLocale(QString locale, QMap<QString, QString> localeStorage)
{
    QRegularExpression regex(locale + "_[A-Z]{2}");
    for (auto &fullLocale : localeStorage.keys())
    {
        QRegularExpressionMatch match = regex.match(fullLocale);
        if (match.hasMatch())
        {
            return localeStorage[fullLocale];
        }
    }
    return localeStorage[""];
}

} // namespace ao_builder
