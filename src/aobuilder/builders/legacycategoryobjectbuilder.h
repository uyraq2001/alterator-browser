#ifndef AOB_LEGACY_CATEGORY_OBJECT_BUILDER_H
#define AOB_LEGACY_CATEGORY_OBJECT_BUILDER_H

#include "baseobjectbuilder.h"

namespace ao_builder
{
class LegacyCategoryObjectBuilder : public BaseObjectBuilder
{
public:
    LegacyCategoryObjectBuilder()           = default;
    ~LegacyCategoryObjectBuilder() override = default;

    std::unique_ptr<Object> buildObject(ObjectParserInterface *parser) override;
};

} // namespace ao_builder

#endif // AOB_LEGACY_CATEGORY_OBJECT_BUILDER_H
