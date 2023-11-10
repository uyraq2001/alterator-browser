#include <QRegularExpression>

#include "category.h"

namespace ab
{
namespace model
{
void Category::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_name = *nameIt;
    }
    else
    {
        QRegularExpression regex(locale + "_[A-Z]{2}");
        for (auto &fullLoc : m_nameLocaleStorage.keys())
        {
            QRegularExpressionMatch match = regex.match(fullLoc);
            if (match.hasMatch())
            {
                m_name = m_nameLocaleStorage[fullLoc];
            }
        }
    }

    auto commentIt = m_commentLocaleStorage.find(locale);
    if (commentIt != m_commentLocaleStorage.end())
    {
        m_comment = *commentIt;
    }
    else
    {
        QRegularExpression regex(locale + "_[A-Z]{2}");
        for (auto &fullLoc : m_commentLocaleStorage.keys())
        {
            QRegularExpressionMatch match = regex.match(fullLoc);
            if (match.hasMatch())
            {
                m_comment = m_commentLocaleStorage[fullLoc];
            }
        }
    }
}
} // namespace model
} // namespace ab
