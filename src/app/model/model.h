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
    Model();
    ~Model() = default;

    void translateModel(QString locale);

private:
    void translateItem(QStandardItem *item, QString locale);

private:
    Model(const Model &) = delete;
    Model(Model &&)      = delete;
    Model &operator=(const Model &) = delete;
    Model &operator=(Model &&) = delete;
};
} // namespace model
} // namespace ab

#endif // AB_MODEL_H
