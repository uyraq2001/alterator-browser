#ifndef AOB_BASIC_OBJECT_H
#define AOB_BASIC_OBJECT_H

#include <QString>

#include "object.h"

namespace ao_builder
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

} // namespace objectbuilder

#endif // AOB_BASIC_OBJECT_H
