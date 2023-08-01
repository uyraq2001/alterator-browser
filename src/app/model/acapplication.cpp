#include "acapplication.h"

ACApplication::ACApplication()
    : m_id()
    , m_name()
    , m_genericName()
    , m_displayName()
    , m_displayComment()
    , m_try_Exec()
    , m_exec()
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

ACApplication::~ACApplication(){}

void ACApplication::setLocale(QString locale)
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

ACObject *ACObject::toObject(){return new ACObject();}
ACCategory *ACObject::toCategory(){return new ACCategory();}
ACInterface *ACObject::toInterface(){return new ACInterface();}
ACApplication *ACObject::toAplication(){return this;}
