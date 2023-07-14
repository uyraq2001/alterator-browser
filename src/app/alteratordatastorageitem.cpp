#include "alteratordatastorageitem.h"

AlteratorDataStorageItem::AlteratorDataStorageItem(QObject *parent)
    : QObject{parent},
      displayData(),
      utilityData()
{}

AlteratorDataStorageItem::~AlteratorDataStorageItem(){}

QVariant AlteratorDataStorageItem::getDisplayData(QString field,
                                                  QString language)
{
    return displayData.value(field, language);
}

QVariant AlteratorDataStorageItem::getDisplayData(QString field)
{
    return utilityData.value(field);
}

void AlteratorDataStorageItem::setDisplayData(QString field,
                                              QString language,
                                              QVariant data)
{
    displayData.insert()
}

void AlteratorDataStorageItem::setDisplayData(QString field,
                                              QVariant data)
{

}
