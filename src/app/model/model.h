#ifndef AB_MODEL_H
#define AB_MODEL_H

#include <QStandardItemModel>

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

#endif // AB_MODEL_H
