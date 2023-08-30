#ifndef AB_CATEGORY_WIDGET_H
#define AB_CATEGORY_WIDGET_H

#include <QStandardItemModel>
#include <QWidget>

#include "controller.h"
#include "pushbutton.h"
#include "model/objectitem.h"

class Controller;
class MainWindow;
class PushButton;

namespace Ui
{
class CategoryWidget;
}

class CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(MainWindow *w, QWidget *parent = nullptr);
    ~CategoryWidget();

    void setGeometry(const QRect &rect);
    void paintEvent(QPaintEvent *event);

    void setItem(ObjectItem *item);

private:
    Ui::CategoryWidget *ui;
    ObjectItem *data;
    MainWindow *window;

    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&)      = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;

signals:

    void showMenu(ObjectItem *item);
};

#endif // AB_CATEGORY_WIDGET_H
