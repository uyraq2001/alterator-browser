#ifndef AB_CATEGORY_WIDGET_H
#define AB_CATEGORY_WIDGET_H

#include "../aobuilder/objects/category.h"
#include "../aobuilder/objects/legacyobject.h"
#include "model/modelinterface.h"

#include <flowlayout.h>
#include <QStandardItemModel>
#include <QWidget>

namespace Ui
{
class CategoryWidget;
}

namespace ab
{
class Controller;
class MainWindow;
class PushButton;

class CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(MainWindow *w,
                            model::ModelInterface *m,
                            ao_builder::Category *cat,
                            QWidget *parent = nullptr);
    ~CategoryWidget() override;

    void setGeometry(const QRect &rect);
    void addObject(ao_builder::Object *object);
    int getWeight();
    bool isEmpty();

public:
    CategoryWidget(const CategoryWidget &)            = delete;
    CategoryWidget(CategoryWidget &&)                 = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&)      = delete;

private:
    Ui::CategoryWidget *ui{};
    ao_builder::Category *category{};
    MainWindow *window{};
    model::ModelInterface *model{};
    FlowLayout *layout{};

signals:
    void showMenu(ao_builder::LegacyObject *obj);
};
} // namespace ab

#endif // AB_CATEGORY_WIDGET_H
