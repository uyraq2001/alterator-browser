#include "localapplicationmodel.h"
#include "localapplicationitem.h"

#include <QDebug>

ACLocalApplicationModel::ACLocalApplicationModel()
    : QStandardItemModel()
{}

ACLocalApplicationModel::~ACLocalApplicationModel() {}

std::vector<ACLocalApplication *> ACLocalApplicationModel::getAppsByInterface(QString interface)
{
    std::vector<ACLocalApplication *> result;

    QStandardItem *rootItem = invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        ACLocalApplicationItem *currentApp = dynamic_cast<ACLocalApplicationItem *>(rootItem->child(i));
        if (!currentApp)
        {
            qWarning() << "Can't cast to ACLocalApplicationItem when try to find apps by interface!";
            continue;
        }

        if (interface == currentApp->getACLocalApplicationObject()->m_implementedInterface)
        {
            result.push_back(currentApp->getACLocalApplicationObject());
        }
    }

    return result;
}

void ACLocalApplicationModel::setLocale() {}
