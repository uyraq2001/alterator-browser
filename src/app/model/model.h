#ifndef AB_MODEL_MODEL_H
#define AB_MODEL_MODEL_H

#include "localapplicationmodel.h"

#include <memory>
#include <QStandardItemModel>

namespace ab
{
namespace model
{
class Model : public QStandardItemModel
{
public:
    void translateModel(QString locale);

    std::unique_ptr<LocalApplicationModel> appModel{}; //(kozyrevid): is it a goog idea to store model in a model?
private:
    void translateItem(QStandardItem *item, QString locale);
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_MODEL_H
