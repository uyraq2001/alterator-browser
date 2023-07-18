#include "acobject.h"

ACObject::ACObject()
    : m_id()
    , m_displayName()
    , m_displayCategory()
    , m_type()
    , m_terminal(false)
    , m_icon()
    , m_x_Alterator_URI()
    , m_x_Alterator_Weight()
    , m_x_Alterator_Help()
    , m_x_Alterator_UI()
    , m_interfaces()
    , m_nameLocaleStorage()
    , m_categoryObject(new ACObjectCategory())
{}

ACObject::~ACObject() {}

void ACObject::setLocale(QString locale)
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
