#include <QMouseEvent>

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
}

bool EditTagDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (index.column() == EditTagDialog::EDIT_TAG_FAVORITE)
		{
			emit SigClickFavorite(index);
		}
	}

	return QItemDelegate::editorEvent(event, model, option, index);
}
