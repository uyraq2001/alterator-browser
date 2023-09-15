#include "abstractentity.h"
#include "entityheaders.h"

namespace ab
{
namespace model
{
AbstractEntity::~AbstractEntity() {}

AbstractEntity *AbstractEntity::createObject()
{
    return new Object();
}
AbstractEntity *AbstractEntity::createCategory()
{
    return new Category();
}
AbstractEntity *AbstractEntity::createInterface()
{
    return new Interface();
}
AbstractEntity *AbstractEntity::createApplication()
{
    return new Application();
}
} // namespace model
} // namespace ab
