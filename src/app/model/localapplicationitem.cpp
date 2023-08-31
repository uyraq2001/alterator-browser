#include "localapplicationitem.h"

namespace ab
{
namespace model
{
LocalApplicationItem::LocalApplicationItem()
    : m_acLocalApplication(nullptr)
{}

LocalApplication *LocalApplicationItem::getLocalApplicationObject()
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
