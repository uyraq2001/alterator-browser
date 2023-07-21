#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QStandardItemModel>
#include <QWidget>

#include "model/acobjectitem.h"
#include "accontroller.h"
#include "acpushbutton.h"

class ACController;

namespace Ui {
    class CategoryWidget;
}

class CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(QWidget *parent = nullptr);
    ~CategoryWidget();

    void setGeometry(const QRect &rect);
    void paintEvent(QPaintEvent *event);

    void onClicked(bool);

    void setItem(ACObjectItem *item);

private:
    Ui::CategoryWidget *ui;

    ACObjectItem *data;

    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&) = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;
};

#endif // CATEGORYWIDGET_H
