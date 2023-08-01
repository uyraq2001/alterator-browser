#include "abstractacentity.h"
#include "model/acobject.h"


AbstractACEntity::~AbstractACEntity(){}

static AbstractACEntity *AbstractACEntity::createObject(){return new ACObject();}
static AbstractACEntity *AbstractACEntity::createCategory(){return new ACCategory();}
static AbstractACEntity *AbstractACEntity::createInterface(){return new ACInterface();}
static AbstractACEntity *AbstractACEntity::createApplication(){return new ACApplication();}
