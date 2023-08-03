#include "fulldefinitions.h"

ACObject::ACObject()
    : m_id()
    , m_displayName()
    , m_displayCategory()
    , m_type()
    , m_categoryId()
    , m_terminal(false)
    , m_icon()
    , m_x_Alterator_URI()
    , m_x_Alterator_Weight()
    , m_x_Alterator_Help()
    , m_x_Alterator_UI()
    , m_nameLocaleStorage()
    , m_isLegacy(false)
{}

ACObject::~ACObject() {}

void ACObject::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }
}

ACObject *ACObject::toObject(){return this;}
ACCategory *ACObject::toCategory(){return new ACCategory();}
ACInterface *ACObject::toInterface(){return new ACInterface();}
ACApplication *ACObject::toAplication(){return new ACApplication();}
