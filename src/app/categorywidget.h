#ifndef AB_CATEGORY_WIDGET_H
#define AB_CATEGORY_WIDGET_H

#include "model/objectitem.h"

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
    explicit CategoryWidget(MainWindow *w, QWidget *parent = nullptr);
    ~CategoryWidget() override;

    void setGeometry(const QRect &rect);
    void setItem(model::ObjectItem *item);

public:
    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&)      = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;

private:
    Ui::CategoryWidget *ui;
    model::ObjectItem *item;
    MainWindow *window;

signals:
    void showMenu(model::ObjectItem *item);
};
} // namespace ab

#endif // AB_CATEGORY_WIDGET_H
