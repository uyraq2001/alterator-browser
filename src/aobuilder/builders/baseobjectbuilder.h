#ifndef AOB_BASE_OBJECT_BUILDER_H
#define AOB_BASE_OBJECT_BUILDER_H

#include "objectbulderinterface.h"

namespace ao_builder
{
class BaseObjectBuilder : public ObjectBuilderInterface
{
public:
    BaseObjectBuilder()           = default;
    ~BaseObjectBuilder() override = default;

public:
    bool buildFieldWithLocale(ObjectParserInterface *parser,
                              QString sectionName,
                              QString entryName,
                              QString &field,
                              QMap<QString, QString> &localeStorage);

    bool buildNames(ObjectParserInterface *parser, QString sectionName, Object *localAppObject);

    std::vector<QString> parseValuesFromKey(ObjectParserInterface *parser,
                                            QString section,
                                            QString key,
                                            QString delimiter);
};
} // namespace ao_builder

#endif // AOB_BASE_OBJECT_BUILDER_H
