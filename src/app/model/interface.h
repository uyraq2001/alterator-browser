#ifndef INTERFACE_H
#define INTERFACE_H

#include "abstractentity.h"
#include "entitydefinitions.h"

namespace ab
{
namespace model
{
class Interface : public AbstractEntity
{
public:
    Interface();
    ~Interface();

    void setLocale(QString locale) override;

    Object *toObject() override;
    Category *toCategory() override;
    Interface *toInterface() override;
    Application *toAplication() override;

    QString m_name;
};
} // namespace model
} // namespace ab

#endif // INTERFACE_H
