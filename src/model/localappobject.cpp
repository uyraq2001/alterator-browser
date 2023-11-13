#include "localappobject.h"

namespace ab
{
namespace model
{
LocalAppObject::LocalAppObject()
    : BasicObject()
{}

LocalAppObject::~LocalAppObject() {}

void LocalAppObject::setLocale(QString locale)
{
    BasicObject::setLocale(locale);

    auto genericNameIt = m_genericNameLocaleStorage.find(locale);
    if (genericNameIt != m_genericNameLocaleStorage.end())
    {
        m_genericName = *genericNameIt;
    }

    auto keywordsIt = m_keywordsLocaleStorage.find(locale);
    if (keywordsIt != m_keywordsLocaleStorage.end())
    {
        m_displayKeywords = *keywordsIt;
    }

    auto commentsIt = m_commentLocaleStorage.find(locale);
    if (commentsIt != m_commentLocaleStorage.end())
    {
        m_displayComment = *commentsIt;
    }
}

} // namespace model
} // namespace ab
