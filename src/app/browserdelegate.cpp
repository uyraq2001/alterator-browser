#include "browserdelegate.h"

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QRect>
#include <QDebug>

BrowserDelegate::BrowserDelegate(QObject *parent
                                 ):QStyledItemDelegate(parent)
{}

void BrowserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
// if (index.column() == 1) {
//        QPushButton *editor = new QPushButton(parent());
        QStyleOptionButton styleOption;
        styleOption.rect = option.rect;
        styleOption.text = index.model()->data(index, Qt::DisplayRole).toString();

        QApplication::style()->drawControl(QStyle::CE_PushButton, &styleOption, painter);
//    } else
//        QStyledItemDelegate::paint(painter, option, index);
}

bool BrowserDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {
    if( event->type() == QEvent::MouseButtonRelease )
        qWarning() << "clicked " + index.model()->data(index).toString();
    return true;
 }
