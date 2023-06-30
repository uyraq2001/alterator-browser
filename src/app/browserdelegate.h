#ifndef BROWSERDELEGATE_H
#define BROWSERDELEGATE_H

#include <QStyledItemDelegate>

class BrowserDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    BrowserDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

   bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};


#endif // BROWSERDELEGATE_H
