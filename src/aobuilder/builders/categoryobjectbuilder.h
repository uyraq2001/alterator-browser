#ifndef AOB_CATEGORY_OBJECT_BUILDER_H
#define AOB_CATEGORY_OBJECT_BUILDER_H

#include "baseobjectbuilder.h"

namespace ao_builder
{
class CategoryObjectBuilder : public BaseObjectBuilder
{
public:
    CategoryObjectBuilder();
    ~CategoryObjectBuilder();

    std::unique_ptr<Object> buildObject(ObjectParserInterface *parser) override;
};

} // namespace ao_builder

#endif // AOB_CATEGORY_OBJECT_BUILDER_H
