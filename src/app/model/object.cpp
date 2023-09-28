#include "object.h"

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
    test += 1;
}
} // namespace model
} // namespace ab
