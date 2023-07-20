#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QStandardItemModel>
#include <QWidget>

#include "model/acobjectitem.h"
#include "accontroller.h"

class ACController;

namespace Ui {
    class CategoryWidget;
}

class CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(ACObjectItem *item,
                            QWidget *parent = nullptr);
    ~CategoryWidget();

    void setGeometry(const QRect &rect);
    void paintEvent(QPaintEvent *event);
//    bool eventFilter(QObject *watched, QEvent *event);

    void setController(ACController *c);

    void onClicked(bool);

private:
    Ui::CategoryWidget *ui;

    ACController *controller;

    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&) = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;
};

#endif // CATEGORYWIDGET_H
