#ifndef ABSTRACTACENTITY_H
#define ABSTRACTACENTITY_H

#include <QObject>

#include "definiciones.h"

class AbstractACEntity : public QObject
{
    Q_OBJECT
public:
    ~AbstractACEntity();

    virtual ACObject *toObject() = 0;
    virtual ACCategory *toCategory() = 0;
    virtual ACInterface *toInterface() = 0;
    virtual ACApplication *toAplication() = 0;

    static AbstractACEntity *createObject();
    static AbstractACEntity *createCategory();
    static AbstractACEntity *createInterface();
    static AbstractACEntity *createApplication();

    virtual void setLocale(QString locale) = 0;

signals:

};

#endif // ABSTRACTACENTITY_H
