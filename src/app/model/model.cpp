#include "model.h"
#include "objectitem.h"

#include <QDebug>

namespace ab
{
namespace model
{
Model::Model()
    : QStandardItemModel()
{}

void Model::translateModel(QString locale)
{
    translateItem(invisibleRootItem(), locale);
}

void Model::translateItem(QStandardItem *item, QString locale)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = item->child(i);
        ObjectItem *currentItem            = dynamic_cast<ObjectItem *>(currentStandardItem);
        if (!currentItem)
        {
            qWarning() << "Can't cast item to ACObjectItem to translate!";

            continue;
        }

        if (currentItem->rowCount() > 0)
        {
            translateItem(currentItem, locale);
        }

        currentItem->getACObject()->setLocale(locale);
    }
}
} // namespace model
} // namespace ab
