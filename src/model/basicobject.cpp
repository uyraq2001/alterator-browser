#include "basicobject.h"

namespace ab
{
namespace model
{
BasicObject::BasicObject()
    : Object()
{}

BasicObject::~BasicObject() {}

void BasicObject::setLocale(QString locale)
{
    Object::setLocale(locale);
}

} // namespace model
} // namespace ab
