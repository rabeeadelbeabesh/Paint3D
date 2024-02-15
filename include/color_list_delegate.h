
#include <QColor>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <sstream>
#include <vtkNamedColors.h>
#include <QPainter>
#include <vtkTransformPolyDataFilter.h>
class ColorListDelegate : public QStyledItemDelegate {
public:
	/**
	 * @brief Constructor for the ColorListDelegate class.
	 *
	 * Initializes a custom delegate for displaying colors in a list widget.
	 *
	 * @param parent Pointer to the parent object.
	 */
	ColorListDelegate(QObject* parent = nullptr);
	/**
	 * @brief Paints the item with the specified color in the list widget.
	 *
	 * Overrides the default paint behavior to display the item with the specified background color.
	 *
	 * @param painter Pointer to the painter object used for drawing.
	 * @param option Style options for the item.
	 * @param index Index of the item in the model.
	 */
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);
};