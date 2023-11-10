#ifndef AB_MODEL_H
#define AB_MODEL_H

#include <QStandardItemModel>

namespace ab
{
namespace model
{
class Model : public QStandardItemModel
{
public:
    void translateModel(QString locale);

private:
    void translateItem(QStandardItem *item, QString locale);
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_H
