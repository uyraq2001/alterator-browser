#include "aclocalapplicationitem.h"

ACLocalApplicationItem::ACLocalApplicationItem()
    : m_acLocalApplication(nullptr)
{}

ACLocalApplication *ACLocalApplicationItem::getACLocalApplicationObject()
{
    return m_acLocalApplication.get();
}
