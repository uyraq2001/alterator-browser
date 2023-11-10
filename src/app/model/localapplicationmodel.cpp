#include "localapplicationmodel.h"
#include "localapplicationitem.h"

#include <set>
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

QStringList LocalApplicationModel::getAllInterfaces()
{
    std::set<QString> res;
    QStandardItem *rootItem = invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        auto currentApp = dynamic_cast<LocalApplicationItem *>(rootItem->child(i));
        if (!currentApp)
        {
            qWarning() << "Can't cast to LocalApplicationItem when try to find apps by interface";
            continue;
        }
        for (auto iface : currentApp->m_acLocalApplication->m_implementedInterfaces)
        {
            res.insert(iface);
        }
    }
    return QStringList(res.begin(), res.end());
}

void LocalApplicationModel::setLocale() {}
} // namespace model
} // namespace ab
