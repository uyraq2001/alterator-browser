#ifndef ABSTRACTACENTITY_H
#define ABSTRACTACENTITY_H

#include <QObject>

#include "entitydefinitions.h"

namespace ab
{
namespace model
{
class AbstractEntity : public QObject
{
    Q_OBJECT
public:
    ~AbstractEntity();

    virtual Object *toObject()          = 0;
    virtual Category *toCategory()      = 0;
    virtual Interface *toInterface()    = 0;
    virtual Application *toAplication() = 0;

    static AbstractEntity *createObject();
    static AbstractEntity *createCategory();
    static AbstractEntity *createInterface();
    static AbstractEntity *createApplication();

    virtual void setLocale(QString locale) = 0;
};
} // namespace model
} // namespace ab
#endif // ABSTRACTACENTITY_H
