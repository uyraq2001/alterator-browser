#include <QRegularExpression>

#include "object.h"

namespace ab
{
namespace model
{
void Object::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }
    else
    {
        QRegularExpression regex(locale + "_[A-Z]{2}");
        for (auto &fullLoc : m_nameLocaleStorage.keys())
        {
            QRegularExpressionMatch match = regex.match(fullLoc);
            if (match.hasMatch())
            {
                m_displayName = m_nameLocaleStorage[fullLoc];
            }
        }
    }
}
} // namespace model
} // namespace ab
