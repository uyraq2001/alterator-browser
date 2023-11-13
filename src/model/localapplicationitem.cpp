#include "localapplicationitem.h"

namespace ab
{
namespace model
{
LocalAppObject *LocalApplicationItem::getLocalApplicationObject()
{
    return m_acLocalApplication.get();
}

void LocalApplicationItem::setLocale(QString locale)
{
    if (m_acLocalApplication)
    {
        m_acLocalApplication->setLocale(locale);
    }
}
} // namespace model
} // namespace ab
