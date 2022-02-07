#include <QMouseEvent>
#include <QLineEdit>
#include <QPainter>

#include "edittagdelegate.h"
#include "edittagdialog.h"

#include "util/log.h"

EditTagDelegate::EditTagDelegate() :
	m_BtnFavorite(new QPushButton)
{

}

EditTagDelegate::~EditTagDelegate()
{
	if (m_BtnFavorite)
	{
		delete m_BtnFavorite;
		m_BtnFavorite = nullptr;
	}
}

void EditTagDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == EditTagDialog::EDIT_TAG_FAVORITE)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x() + (option.rect.width() - 15) / 2,
							 option.rect.y() + (option.rect.height() - 26) / 2,
							 15,
							 26);
//		button.text = "test";
		button.features = QStyleOptionButton::Flat;

		QString style;
		if (index.data().toString().toInt() == 0)
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn12-like-n.png\");	\
							}");
		}
		else
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn12-like-u.png\");	\
							}");
		}
		m_BtnFavorite->setStyleSheet(style);
		m_BtnFavorite->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnFavorite);

	}
	else if (index.column() == EditTagDialog::EDIT_TAG_YEAR)
	{
		QItemDelegate::paint(painter, option, index);
		painter->save();

		QString year = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_YEAR));

		QFont fontYear("Segoe UI", 9, QFont::Normal, false);
		QFontMetrics fmTitle(fontYear);

		QRect rectYear = option.rect;

//		painter->drawRect(rectYear);

		painter->setPen(QColor(84, 84, 84));
		if (!year.isEmpty())
		{
			painter->setFont(fontYear);
			painter->drawText(rectYear, year);
		}

		painter->restore();

	}
}

QWidget *EditTagDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == EditTagDialog::EDIT_TAG_YEAR)
	{
		QLineEdit *lineEdit = new QLineEdit(parent);
		// Set validator
		QIntValidator *validator = new QIntValidator(1900, 2100, lineEdit);
		lineEdit->setValidator(validator);
		return lineEdit;
	}
	else
	{
		return nullptr;
	}
}

bool EditTagDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (index.column() == EditTagDialog::EDIT_TAG_FAVORITE)
			{
				emit SigClickFavorite(index);
			}
		}
	}

	return QItemDelegate::editorEvent(event, model, option, index);
}
