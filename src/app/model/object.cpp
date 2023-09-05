#include "object.h"

namespace ab
{
namespace model
{
Object::Object()
    : m_categoryObject(new ObjectCategory())
{}

void Object::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }

    if (m_categoryObject)
    {
        m_categoryObject->setLocale(locale);
        m_displayCategory = m_categoryObject->m_name;
    }
}
} // namespace model
} // namespace ab
