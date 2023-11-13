#ifndef AB_BASICOBJECT_H
#define AB_BASICOBJECT_H

#include "object.h"

namespace ab
{
namespace model
{
class BasicObject : public Object
{
public:
    BasicObject();
    virtual ~BasicObject();

    virtual void setLocale(QString locale) override;

public:
    QString m_categoryId{};

    bool m_isLegacy{false};
};

} // namespace model
} // namespace ab

#endif // BASICOBJECT_H
