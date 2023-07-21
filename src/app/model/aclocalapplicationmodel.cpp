#include "aclocalapplicationmodel.h"
#include "aclocalapplicationitem.h"

#include <QDebug>

ACLocalApplicationModel::ACLocalApplicationModel()
    : QStandardItemModel()
{}

ACLocalApplicationModel::~ACLocalApplicationModel() {}

void ACLocalApplicationModel::setLocale(QString locale)
{
    QStandardItem *rootStandardItem = invisibleRootItem();

    for (int i = 0; i < rootStandardItem->rowCount(); ++i)
    {
        ACLocalApplicationItem *currentItem = dynamic_cast<ACLocalApplicationItem *>(rootStandardItem->child(i));

        if (!currentItem)
        {
            qWarning() << "WARNING! Can't cast standard item to ACLocalApplicationItem to translate!";

            continue;
        }

        currentItem->setLocale(locale);
    }
}
