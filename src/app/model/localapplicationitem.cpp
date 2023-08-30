#include "localapplicationitem.h"

ACLocalApplicationItem::ACLocalApplicationItem()
    : m_acLocalApplication(nullptr)
{}

ACLocalApplication *ACLocalApplicationItem::getACLocalApplicationObject()
{
    return m_acLocalApplication.get();
}

void ACLocalApplicationItem::setLocale(QString locale)
{
    if (m_acLocalApplication)
    {
        m_acLocalApplication->setLocale(locale);
    }
}
