#include "localapplicationitem.h"

LocalApplicationItem::LocalApplicationItem()
    : m_acLocalApplication(nullptr)
{}

LocalApplication *LocalApplicationItem::getACLocalApplicationObject()
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
