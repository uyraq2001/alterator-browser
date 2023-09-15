#include "entityheaders.h"

namespace ab
{
namespace model
{
Interface::Interface()
    : m_name()
{}

Interface::~Interface() {}

void Interface::setLocale(QString locale) {}

Object *Interface::toObject()
{
    return new Object();
}
Category *Interface::toCategory()
{
    return new Category();
}
Interface *Interface::toInterface()
{
    return this;
}
Application *Interface::toAplication()
{
    return new Application();
}
} // namespace model
} // namespace ab
