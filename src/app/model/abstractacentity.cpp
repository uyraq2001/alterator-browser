#include "abstractacentity.h"
#include "fulldefinitions.h"


AbstractACEntity::~AbstractACEntity(){}

AbstractACEntity *AbstractACEntity::createObject(){return new ACObject();}
AbstractACEntity *AbstractACEntity::createCategory(){return new ACCategory();}
AbstractACEntity *AbstractACEntity::createInterface(){return new ACInterface();}
AbstractACEntity *AbstractACEntity::createApplication(){return new ACApplication();}
