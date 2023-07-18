#include "acobjectcategory.h"

ACObjectCategory::ACObjectCategory()
    : m_id()
    , m_name()
    , m_comment()
    , m_icon()
    , m_type()
    , m_xAlteratorCategory()
    , m_nameLocaleStorage()
    , m_commentLocaleStorage()
{}

ACObjectCategory::~ACObjectCategory() {}

void ACObjectCategory::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_name = *nameIt;
    }

    auto commentIt = m_commentLocaleStorage.find(locale);
    if (commentIt != m_commentLocaleStorage.end())
    {
        m_comment = *commentIt;
    }
}
