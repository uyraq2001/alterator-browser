#include "localapplicationmodel.h"
#include "localapplicationitem.h"

#include <QDebug>

namespace ab
{
namespace model
{
std::vector<LocalApplication *> LocalApplicationModel::getAppsByInterface(QString interface)
{
    std::vector<LocalApplication *> result;

    QStandardItem *rootItem = invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        auto currentApp = dynamic_cast<LocalApplicationItem *>(rootItem->child(i));
        if (!currentApp)
        {
            qWarning() << "Can't cast to LocalApplicationItem when try to find apps by interface";
            continue;
        }

        for (auto &iface : currentApp->getLocalApplicationObject()->m_implementedInterfaces)
        {
            if (interface == iface)
            {
                result.push_back(currentApp->getLocalApplicationObject());
                continue;
            }
        }
    }

    return result;
}

void LocalApplicationModel::setLocale() {}
} // namespace model
} // namespace ab
