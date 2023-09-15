#include "entityheaders.h"

namespace ab
{
namespace model
{
Object::Object()
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
    , m_interfaces()
    , m_applications()
    , m_nameLocaleStorage()
    , m_isLegacy(false)
{}

void Object::setLocale(QString locale)
{
    auto nameIt = m_nameLocaleStorage.find(locale);
    if (nameIt != m_nameLocaleStorage.end())
    {
        m_displayName = *nameIt;
    }

    //    if (m_categoryObject)
    //    {
    //        m_categoryObject->setLocale(locale);
    //        m_displayCategory = m_categoryObject->m_name;
    //    }
}

Object *Object::toObject()
{
    return this;
}
Category *Object::toCategory()
{
    return new Category();
}
Interface *Object::toInterface()
{
    return new Interface();
}
Application *Object::toAplication()
{
    return new Application();
}

} // namespace model
} // namespace ab
