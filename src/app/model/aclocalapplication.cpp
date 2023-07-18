#include "aclocalapplication.h"

ACLocalApplication::ACLocalApplication()
    : m_id()
    , m_name()
    , m_genericName()
    , m_comment()
    , m_type()
    , m_icon()
    , m_exec()
    , m_termminal()
    , m_categories()
    , m_mimeType()
    , m_currentKeywords()
    , m_startupNotify(false)
    , m_keywords()
    , m_nameLocaleStorage()
    , m_genericNameLocaleStorage()
    , m_commentLocaleStorage()
{}

ACLocalApplication::~ACLocalApplication() {}

void ACLocalApplication::setLocale(QString locale)
{
    auto keywordsIt = m_keywords.find(locale);
    if (keywordsIt != m_keywords.end())
    {
        std::copy((*keywordsIt).begin(), (*keywordsIt).end(), m_currentKeywords.begin());
    }

    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_name = *nameIt;
    }

    auto m_genericNameIt = m_genericNameLocaleStorage.find(locale);
    if (m_genericNameIt != m_genericNameLocaleStorage.end())
    {
        m_genericName = *m_genericNameIt;
    }

    auto commentIt = m_commentLocaleStorage.find(locale);
    if (commentIt != m_commentLocaleStorage.end())
    {
        m_comment = *commentIt;
    }
}
