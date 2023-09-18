#include "entityheaders.h"

namespace ab
{
namespace model
{
void Application::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }

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

Object *Application::toObject()
{
    return new Object();
}
Category *Application::toCategory()
{
    return new Category();
}
Interface *Application::toInterface()
{
    return new Interface();
}
Application *Application::toAplication()
{
    return this;
}
} // namespace model
} // namespace ab
