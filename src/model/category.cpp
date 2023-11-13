#include <QRegularExpression>

#include "category.h"

namespace ab
{
namespace model
{
Category::Category()
    : Object()
{}

Category::~Category() {}

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
