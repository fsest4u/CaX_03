#include <QMouseEvent>

#include "tabletracksdelegate.h"
#include "tabletracks.h"

#include "util/caxconstants.h"
#include "util/log.h"
#include "util/utilnovatron.h"

TableTracksDelegate::TableTracksDelegate() :
	m_BtnSelect(new QPushButton),
	m_BtnCover(new QPushButton),
	m_BtnPlay(new QPushButton),
	m_BtnFavorite(new QPushButton),
	m_BtnRating(new QPushButton),
	m_BtnMenu(new QPushButton)
{
	ConnectSigToSlot();
	Initialize();
}

TableTracksDelegate::~TableTracksDelegate()
{
	if (m_BtnSelect)
	{
		delete m_BtnSelect;
		m_BtnSelect = nullptr;
	}

	if (m_BtnCover)
	{
		delete m_BtnCover;
		m_BtnCover = nullptr;
	}

	if (m_BtnPlay)
	{
		delete m_BtnPlay;
		m_BtnPlay = nullptr;
	}

	if (m_BtnFavorite)
	{
		delete m_BtnFavorite;
		m_BtnFavorite = nullptr;
	}

	if (m_BtnRating)
	{
		delete m_BtnRating;
		m_BtnRating = nullptr;
	}

	if (m_BtnMenu)
	{
		delete m_BtnMenu;
		m_BtnMenu = nullptr;
	}
}

void TableTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == TableTracks::TABLE_TRACKS_SELECT)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x() + (option.rect.width() - 20) / 2,
							 option.rect.y() + (option.rect.height() - 20) / 2,
							 20,
							 20);
//		button.text = "test";
		button.features = QStyleOptionButton::Flat;

		QString style;
		if (index.data().toBool())
		{
			style = QString("	\
						   QPushButton	\
						   {	\
							   border-image: url(\":/resource/playlist-btn30-selecton-h.png\");	\
						   }");
		}
		else
		{
			style = QString("	\
						   QPushButton	\
						   {	\
							   border-image: url(\":/resource/playlist-btn30-selecton-n.png\");	\
						   }");
		}

		m_BtnSelect->setStyleSheet(style);
		m_BtnSelect->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnSelect);
	}
	else if (index.column() == TableTracks::TABLE_TRACKS_COVER)
	{
		QString coverArt = index.data().toString();
//		LogDebug("cover art [%s]", coverArt.toUtf8().data());

		int length = option.rect.width() >= option.rect.height() ? option.rect.height() - 5 : option.rect.width() - 5;
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x() + (option.rect.width() - length) / 2,
							 option.rect.y() + (option.rect.height() - length) / 2,
							 length,
							 length);
		button.features = QStyleOptionButton::Flat;

		QString style = QString("	\
						   QPushButton	\
						   {	\
							   border-image: url(\"%1\");	\
						   }").arg(coverArt);

		m_BtnSelect->setStyleSheet(style);
		m_BtnSelect->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnSelect);
	}
	else if (index.column() == TableTracks::TABLE_TRACKS_PLAY)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x() + (option.rect.width() - 20) / 2,
							 option.rect.y() + (option.rect.height() - 20) / 2,
							 20,
							 20);
		button.features = QStyleOptionButton::Flat;

		QString style = QString("	\
						QPushButton	\
						{	\
							border-image: url(\":/resource/play-btn20-play-icon.png\");	\
						}");
		m_BtnPlay->setStyleSheet(style);
		m_BtnPlay->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnPlay);
	}
	else if (index.column() == TableTracks::TABLE_TRACKS_FAVORITE)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x(),
							 option.rect.y() + (option.rect.height() - 22) / 2,
							 22,
							 22);
		button.features = QStyleOptionButton::Flat;

		QString style;
		if (index.data().toString().toInt() == 0)
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn28-like-n.png\");	\
							}");
		}
		else
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn28-like-h.png\");	\
							}");
		}
		m_BtnFavorite->setStyleSheet(style);
		m_BtnFavorite->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnFavorite);

	}
	else if (index.column() == TableTracks::TABLE_TRACKS_RATING)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x(),
							 option.rect.y() + (option.rect.height() - 22) / 2,
							 86,
							 22);
		button.features = QStyleOptionButton::Flat;

		QString style;
//		LogDebug("rating ~~~~~~~~~~~~~[%s]", index.data().toString().toUtf8().data());
		if (index.data().toString().toInt() <= 0)
		{
			style = "play-btn28-rank-sel0.png";

		}
		else if (index.data().toString().toInt() == 1)
		{
			style = "play-btn28-rank-sel1.png";

		}
		else if (index.data().toString().toInt() == 2)
		{
			style = "play-btn28-rank-sel2.png";

		}
		else if (index.data().toString().toInt() == 3)
		{
			style = "play-btn28-rank-sel3.png";

		}
		else if (index.data().toString().toInt() == 4)
		{
			style = "play-btn28-rank-sel4.png";

		}
		else if (index.data().toString().toInt() == 5)
		{
			style = "play-btn28-rank-sel5.png";

		}

		style = QString("	\
						QPushButton	\
						{	\
						  border-image: url(\":/resource/%1\");	\
						}").arg(style);

		m_BtnRating->setStyleSheet(style);
		m_BtnRating->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnRating);

	}
	else if (index.column() == TableTracks::TABLE_TRACKS_MENU)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x() + (option.rect.width() - 28) / 2,
							 option.rect.y() + (option.rect.height() - 28) / 2,
							 28,
							 28);
		button.features = QStyleOptionButton::Flat;

		QString style = QString("	\
								QPushButton	\
								{	\
								  border-image: url(\":/resource/play-btn28-popupmenu-n.png\");	\
								}	\
								QPushButton:hover	\
								{	\
								  border-image: url(\":/resource/play-btn28-popupmenu-h.png\");	\
								}	\
								QPushButton::menu-indicator	\
								{	\
									image:none;	\
								}");

		m_BtnMenu->setStyleSheet(style);
		m_BtnMenu->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnMenu);

	}
}

bool TableTracksDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{

		}
	}

	return QItemDelegate::editorEvent(event, model, option, index);
}

void TableTracksDelegate::ConnectSigToSlot()
{

}

void TableTracksDelegate::Initialize()
{

}
