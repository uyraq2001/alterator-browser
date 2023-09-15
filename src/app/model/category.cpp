#include "entityheaders.h"

namespace ab
{
namespace model
{
Category::Category()
    : m_id()
    , m_name()
    , m_comment()
    , m_icon()
    , m_type()
    , m_xAlteratorCategory()
    , m_nameLocaleStorage()
    , m_commentLocaleStorage()
{}

Category::~Category() {}

Object *Category::toObject()
{
    return new Object();
}
Category *Category::toCategory()
{
    return this;
}
Interface *Category::toInterface()
{
    return new Interface();
}
Application *Category::toAplication()
{
    return new Application();
}

void Category::setLocale(QString locale)
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
} // namespace model
} // namespace ab
