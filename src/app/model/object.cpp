#include "entityheaders.h"

namespace ab
{
namespace model
{
void Object::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }
}

Object *Object::toObject()
{
    return this;
}
Category *Object::toCategory()
{
    return new Category();
}
Interface *Object::toInterface()
{
    return new Interface();
}
Application *Object::toAplication()
{
    return new Application();
}

} // namespace model
} // namespace ab
