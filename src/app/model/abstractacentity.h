#ifndef ABSTRACTACENTITY_H
#define ABSTRACTACENTITY_H

#include <QObject>

class AbstractACEntity : public QObject
{
    Q_OBJECT
public:
    ~AbstractACEntity();

    virtual ACObject toObject() = 0;
    virtual ACCategory toCategory() = 0;
    virtual ACInterface toInterface() = 0;
    virtual ACApplication toAplication() = 0;

    AbstractACEntity createObject();
    AbstractACEntity createCategory();
    AbstractACEntity createInterface();
    AbstractACEntity createApplication();

signals:

};

#endif // ABSTRACTACENTITY_H
