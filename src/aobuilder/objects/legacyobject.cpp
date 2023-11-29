#include "legacyobject.h"
namespace ao_builder
{
LegacyObject::LegacyObject() {}

LegacyObject::~LegacyObject() {}

void LegacyObject::setLocale(QString locale)
{
    BasicObject::setLocale(locale);
}

} // namespace ao_builder
