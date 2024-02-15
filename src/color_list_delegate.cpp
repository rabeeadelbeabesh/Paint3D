#include "color_list_delegate.h"


ColorListDelegate::ColorListDelegate(QObject* parent):QStyledItemDelegate(parent)
{
}

void ColorListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	QStyledItemDelegate::paint(painter, option, index);

	// Get the color associated with the item
	QColor color = index.data(Qt::BackgroundRole).value<QColor>();

	// Set the background color
	painter->fillRect(option.rect, color);
}
