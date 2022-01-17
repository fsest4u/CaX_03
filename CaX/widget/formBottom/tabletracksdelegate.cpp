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

	if (m_BtnMenu)
	{
		delete m_BtnMenu;
		m_BtnMenu = nullptr;
	}
}

void TableTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int col = index.column();
	if (col == TableTracks::TABLE_TRACKS_SELECT)
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
		button.rect  = QRect(option.rect.x() + (option.rect.width() - 15) / 2,
							 option.rect.y() + (option.rect.height() - 26) / 2,
							 15,
							 26);
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
	}

	return QItemDelegate::editorEvent(event, model, option, index);
}

void TableTracksDelegate::ConnectSigToSlot()
{

}

void TableTracksDelegate::Initialize()
{

}
