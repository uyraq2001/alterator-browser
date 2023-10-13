#ifndef MULTIENTITYBUILDER_H
#define MULTIENTITYBUILDER_H

#include "category.h"
#include "desktopfileparser.h"
#include "localapplication.h"
#include "object.h"

#include <memory>
#include <variant>
#include <vector>

namespace ab
{
namespace model
{
class MultiEntityBuilder
{
public:
    MultiEntityBuilder(DesktopFileParser *infoParser);

    std::vector<std::unique_ptr<std::variant<Object, Category, LocalApplication>>> buildAll();

private:
    DesktopFileParser *m_infoParser = nullptr;
};
} // namespace model
} // namespace ab

#endif // MULTIENTITYBUILDER_H
