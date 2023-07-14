#ifndef ALTERATORDATASTORAGEITEM_H
#define ALTERATORDATASTORAGEITEM_H

#include <QObject>
#include <QVariant>

class AlteratorDataStorageItem : public QObject
{
    Q_OBJECT
public:
    explicit AlteratorDataStorageItem(QObject *parent = nullptr);
    ~AlteratorDataStorageItem();

    QVariant getDisplayData(QString field,
                            QString language);
    QVariant getDisplayData(QString field);

    void setDisplayData(QString field,
                            QString language,
                            QVariant data);
    void setDisplayData(QString field,
                            QVariant data);

private:
    QMap<QString, QVariantMap> displayData;
    QVariantMap utilityData;

signals:

};

#endif // ALTERATORDATASTORAGEITEM_H
