#ifndef NEWOBJECTBUILDER_H
#define NEWOBJECTBUILDER_H

#include "baseobjectbuilder.h"

namespace ao_builder
{
class NewObjectBuilder : public BaseObjectBuilder
{
public:
    NewObjectBuilder()           = default;
    ~NewObjectBuilder() override = default;

    std::unique_ptr<Object> buildObject(ObjectParserInterface *parser) override;
};
} // namespace ao_builder
#endif // NEWOBJECTBUILDER_H
