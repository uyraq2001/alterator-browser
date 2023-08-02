#include "aclocalapplication.h"

ACLocalApplication::ACLocalApplication()
    : m_id()
    , m_name()
    , m_genericName()
    , m_displayName()
    , m_displayComment()
    , m_try_Exec()
    , m_desktopExec()
    , m_icon()
    , m_type()
    , m_displayKeywords()
    , m_implementedInterface()
    , m_categories()
    , m_mimeTypes()
    , m_alteratorIterfaces()
    , m_nameLocaleStorage()
    , m_genericNameLocaleStorage()
    , m_keywordsLocaleStorage()
    , m_commentLocaleStorage()

{}

ACLocalApplication::~ACLocalApplication() {}

void ACLocalApplication::setLocale(QString locale)
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
