#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QStandardItemModel>
#include <QWidget>

#include "model/acobjectitem.h"
#include "accontroller.h"
#include "acpushbutton.h"

class ACController;
class MainWindow;
class ACPushButton;

namespace Ui {
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

    void setItem(ACObjectItem *item);

private:
    Ui::CategoryWidget *ui;
    ACObjectItem *data;
    MainWindow *window;

    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&) = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;

signals:

    void showMenu(ACObjectItem *item);
};

#endif // CATEGORYWIDGET_H
