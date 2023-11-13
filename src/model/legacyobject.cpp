#include "legacyobject.h"

namespace ab
{
namespace model
{
LegacyObject::LegacyObject()
    : BasicObject()
{}

LegacyObject::~LegacyObject() {}

void LegacyObject::setLocale(QString locale)
{
    BasicObject::setLocale(locale);
}

} // namespace model
} // namespace ab
