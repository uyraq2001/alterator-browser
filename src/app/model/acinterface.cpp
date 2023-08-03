#include "fulldefinitions.h"

ACInterface::ACInterface()
    : m_name()
{}

ACInterface::~ACInterface(){}

void ACInterface::setLocale(QString locale){}

ACObject *ACInterface::toObject(){return new ACObject();}
ACCategory *ACInterface::toCategory(){return new ACCategory();}
ACInterface *ACInterface::toInterface(){return this;}
ACApplication *ACInterface::toAplication(){return new ACApplication();}
