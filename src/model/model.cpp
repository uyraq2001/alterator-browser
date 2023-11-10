#include "model.h"
#include "category.h"
#include "object.h"
#include "objectitem.h"

#include <QDebug>

#include <variant>

template<typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

namespace ab
{
namespace model
{
void Model::translateModel(QString locale)
{
    translateItem(invisibleRootItem(), locale);
}

void Model::translateItem(QStandardItem *item, QString locale)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *currentStandardItem = item->child(i);
        auto currentItem                   = dynamic_cast<ObjectItem *>(currentStandardItem);
        if (!currentItem)
        {
            qWarning() << "Can't cast item to ObjectItem to translate";
            continue;
        }

        if (currentItem->rowCount() > 0)
        {
            translateItem(currentItem, locale);
        }

        std::visit(Overload{[locale](auto &&obj) { obj.setLocale(locale); }}, *currentItem->getObject());
    }
}
} // namespace model
} // namespace ab
