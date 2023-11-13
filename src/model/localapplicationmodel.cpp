#include "localapplicationmodel.h"
#include "localapplicationitem.h"

#include <QDebug>

namespace ab
{
namespace model
{
std::vector<LocalAppObject *> LocalApplicationModel::getAppsByInterface(QString interface)
{
    std::vector<LocalAppObject *> result;

    QStandardItem *rootItem = invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        auto currentApp = dynamic_cast<LocalAppObject *>(rootItem->child(i));
        if (!currentApp)
        {
            qWarning() << "Can't cast to LocalApplicationItem when try to find apps by interface";
            continue;
        }

        //        if (interface == currentApp->getLocalApplicationObject()->m_implementedInterface)
        //        {
        //            result.push_back(currentApp->getLocalApplicationObject());
        //        }
    }

    return result;
}

void LocalApplicationModel::setLocale() {}
} // namespace model
} // namespace ab
