#include "localapplicationmodel.h"
#include "localapplicationitem.h"

#include <QDebug>

namespace ab
{
namespace model
{
LocalApplicationModel::LocalApplicationModel()
    : QStandardItemModel()
{}

LocalApplicationModel::~LocalApplicationModel() {}

std::vector<LocalApplication *> LocalApplicationModel::getAppsByInterface(QString interface)
{
    std::vector<LocalApplication *> result;

    QStandardItem *rootItem = invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        LocalApplicationItem *currentApp = dynamic_cast<LocalApplicationItem *>(rootItem->child(i));
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

void LocalApplicationModel::setLocale() {}
} // namespace model
} // namespace ab
