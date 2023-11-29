#ifndef AOB_BASIC_OBJECT_H
#define AOB_BASIC_OBJECT_H

#include <QString>

#include "object.h"

namespace ao_builder
{
class BasicObject : public Object
{
public:
    BasicObject()           = default;
    ~BasicObject() override = default;

    void setLocale(QString locale) override;

public:
    QString m_categoryId{};
    bool m_isLegacy{false};
};
} // namespace ao_builder

#endif // AOB_BASIC_OBJECT_H
