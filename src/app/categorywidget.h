#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QStandardItemModel>
#include <QWidget>



namespace Ui {
    class CategoryWidget;
}


class CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(QStandardItemModel *m, int row, QWidget *parent = nullptr);
    ~CategoryWidget();

    void setGeometry(const QRect &rect);
    void paintEvent(QPaintEvent *event);

private:
    Ui::CategoryWidget *ui;

    CategoryWidget(const CategoryWidget &) = delete;
    CategoryWidget(CategoryWidget &&)      = delete;
    CategoryWidget &operator=(const CategoryWidget &) = delete;
    CategoryWidget &operator=(CategoryWidget &&) = delete;
};

#endif // CATEGORYWIDGET_H
