#ifndef AOB_OBJECT_BUILDER_FACTORY_H
#define AOB_OBJECT_BUILDER_FACTORY_H

#include "objectbulderinterface.h"

namespace ao_builder
{
class ObjectBuilderFactory
{
public:
    ObjectBuilderFactory()  = default;
    ~ObjectBuilderFactory() = default;

    std::unique_ptr<ObjectBuilderInterface> getBuilder(ObjectParserInterface *parser);

private:
    std::unique_ptr<ObjectBuilderInterface> getObjectBuilder(ObjectParserInterface *parser);
};

} // namespace ao_builder

#endif // AOB_OBJECT_BUILDER_FACTORY_H
