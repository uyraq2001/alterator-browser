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
} // namespace model
} // namespace ab
