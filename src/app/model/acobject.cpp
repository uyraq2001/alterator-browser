#include "acobject.h"

ACObject::ACObject()
    : m_id()
    , m_name()
    , m_category()
    , m_nameLocaleStorage()
    , m_categoryLocaleStorage()
{}

ACObject::~ACObject() {}

void ACObject::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_name = *nameIt;
    }

    auto categoryIt = m_categoryLocaleStorage.find(locale);
    if (categoryIt != m_categoryLocaleStorage.end())
    {
        m_category = *categoryIt;
    }
}
