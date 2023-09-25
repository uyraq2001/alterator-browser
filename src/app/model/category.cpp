#include "entityheaders.h"

namespace ab
{
namespace model
{
//Object *Category::toObject()
//{
//    return new Object();
//}
//Category *Category::toCategory()
//{
//    return this;
//}
//Interface *Category::toInterface()
//{
//    return new Interface();
//}
//Application *Category::toAplication()
//{
//    return new Application();
//}

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
