#include "basicobject.h"

namespace ao_builder
{
BasicObject::BasicObject()
    : Object()
{}

BasicObject::~BasicObject() {}

void BasicObject::setLocale(QString locale)
{
    Object::setLocale(locale);
}

} // namespace objectbuilder
