#include "acmodel.h"
#include "acobjectitem.h"

#include <QDebug>

ACModel::ACModel()
    : QStandardItemModel()
{}

void ACModel::translateModel(QString locale)
{
    QStandardItem *rootItem = invisibleRootItem();

    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = rootItem->child(i);
        ACObjectItem *currentItem          = dynamic_cast<ACObjectItem *>(currentStandardItem);
        if (!currentItem)
        {
            qWarning() << "WARNING! Can't cast category item to ACObjectItem to translate!";

            continue;
        }

        translateItem(this->item(i), locale);

        auto t = currentItem->getACEntity();
        t->toCategory();
        t->setLocale(locale);
    }
}

void ACModel::translateItem(QStandardItem *item, QString locale)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = item->child(i);
        ACObjectItem *currentItem          = dynamic_cast<ACObjectItem *>(currentStandardItem);
        if (!currentItem)
        {
            qWarning() << "WARNING! Can't cast item to ACObjectItem to translate!";

            continue;
        }

        if (currentItem->rowCount() > 0)
        {
            translateItem(currentItem, locale);
        }

        currentItem->getACEntity()->setLocale(locale);
    }
}
