#include <QPainter>

#include "listbrowserdelegate.h"
//#include "listbrowsereditor.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/airable.h"
#include "widget/browser.h"
#include "widget/qobuz.h"

//#include "widget/form/formcoverart.h"

ListBrowserDelegate::ListBrowserDelegate()
{
}

int ListBrowserDelegate::GetService() const
{
	return m_Service;
}

void ListBrowserDelegate::SetService(int Service)
{
	m_Service = Service;
}

//QMap<int, QString> ListBrowserDelegate::GetOptionMenuMap() const
//{
//	return m_OptionMenuMap;
//}

//void ListBrowserDelegate::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
//{
//	m_OptionMenuMap = OptionMenuMap;
//}

//void ListBrowserDelegate::SlotClickCoverArt(int index)
//{
//	emit SigSelectCoverArt(index);
//}

//void ListBrowserDelegate::SlotClickPlay(int type, CJsonNode node)
//{
//	emit SigSelectPlay(type, node);
//}

//void ListBrowserDelegate::SlotClickTitle(int type, CJsonNode node)
//{
//	emit SigSelectTitle(type, node);
//}

//void ListBrowserDelegate::SlotMenu(int index, int type)
//{
//	emit SigMenu(index, type);
//}

//void ListBrowserDelegate::SlotMenu(int index, int type, QString menuName)
//{
//	emit SigMenu(index, type, menuName);
//}

//void ListBrowserDelegate::SlotMenuAction(QString path, int type, int menuID)
//{
//	emit SigMenuAction(path, type, menuID);
//}

//void ListBrowserDelegate::SlotMenuAction(CJsonNode node, int type, int menuID)
//{
//	emit SigMenuAction(node, type, menuID);
//}

void ListBrowserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	int type = qvariant_cast<int>(index.data(LIST_BROWSER_TYPE));
	QString cover = qvariant_cast<QString>(index.data(LIST_BROWSER_COVER));
	QString title = qvariant_cast<QString>(index.data(LIST_BROWSER_TITLE));
	QString subtitle = qvariant_cast<QString>(index.data(LIST_BROWSER_SUBTITLE));
	QString duration = qvariant_cast<QString>(index.data(LIST_BROWSER_DURATION));
	QString filesize = qvariant_cast<QString>(index.data(LIST_BROWSER_FILESIZE));
	bool select = qvariant_cast<bool>(index.data(LIST_BROWSER_SELECT));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int indent = 0;
	if (SIDEMENU_ISERVICE == m_Service)
	{
		if (iAirableType_Mask_Sub & type)
		{
			indent = 40;
		}
	}
	QRect rectCover = QRect(rectBase.x() + 60 + indent, rectBase.y() + (rectBase.height() - 40) / 2, 40, 40);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 16 - 2, rectCover.y() + 2, 16, 16);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + 15, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + 15, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 400, fmTitle.height());
	int gap = ( rectBase.width() - rectTitle.x() - (400 + 300 + 100 + 100 + 30 + 60) ) / 4;

	QRect rectSubtitle = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 300, fmTitle.height());
	QRect rectDuration = QRect(rectSubtitle.x() + rectSubtitle.width() + gap, rectTitle.y(), 100, fmTitle.height());
	QRect rectFileSize = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 100, fmTitle.height());
//	QRect rectMenu = QRect(rectFileSize.x() + rectFileSize.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 60 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCover);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectSubtitle);
//	painter->drawRect(rectDuration);
//	painter->drawRect(rectFileSize);
//	painter->drawRect(rectMenu);

	QPixmap pixCover;
	if (cover.isEmpty())
	{
		cover = QString(":/resource/playlist-img160-albumart-h.png");
	}
	if (pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}

	QPixmap pixCheck;
	QString resCheck;
	if (select)
	{
		resCheck = QString(":/resource/playlist-btn30-selecton-h.png");
	}
	else
	{
		resCheck = QString(":/resource/playlist-btn30-selecton-n.png");
	}
	if (pixCheck.load(resCheck))
	{
		painter->drawPixmap(rectCheck, pixCheck);
	}

	QPixmap pixPlay;
	QString resPlay = QString(":/resource/browser-icon16-playnow.png");
	if (SIDEMENU_BROWSER == m_Service)
	{
		if (iFolderType_Mask_Play_Select & type)
		{
			if (pixPlay.load(resPlay))
			{
				painter->drawPixmap(rectPlay, pixPlay);
			}
		}
	}
	else if (SIDEMENU_ISERVICE == m_Service)
	{
		if (iAirableType_Mask_Track & type
				|| iAirableType_Mask_Program & type
				|| iAirableType_Mask_Radio & type
				|| iAirableType_Mask_Feed & type
				|| iAirableType_Mask_Play & type)
		{
			if (pixPlay.load(resPlay))
			{
				painter->drawPixmap(rectPlay, pixPlay);
			}
		}
	}
	else
	{
		if (iQobuzType_Mask_Track & type)
		{
			if (pixPlay.load(resPlay))
			{
				painter->drawPixmap(rectPlay, pixPlay);
			}
		}
	}

	QPixmap pixMenu;
	QString resMenu = QString(":/resource/play-btn28-popupmenu-n.png");
	if (SIDEMENU_BROWSER == m_Service)
	{
		if (pixMenu.load(resMenu))
		{
			painter->drawPixmap(rectMenu, pixMenu);
		}
	}
	else if (SIDEMENU_ISERVICE == m_Service)
	{
		if (iAirableType_Mask_Art & type)
		{
			if (pixMenu.load(resMenu))
			{
				painter->drawPixmap(rectMenu, pixMenu);
			}
		}
	}
	else
	{
		if ((iQobuzType_Mask_UserPlaylist | iQobuzType_Mask_Playlist) == type)
		{
			if (pixMenu.load(resMenu))
			{
				painter->drawPixmap(rectMenu, pixMenu);
			}
		}
	}

	if (SIDEMENU_BROWSER == m_Service && (type & iFolderType_Mask_Scan))
	{
		painter->setPen(QColor(52, 174, 214));
	}
	else
	{
		painter->setPen(QColor(84, 84, 84));
	}

	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!subtitle.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectSubtitle, subtitle);
	}
	if (!duration.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectDuration, duration);
	}
	if (!filesize.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectFileSize, filesize);
	}

	painter->restore();
}

QSize ListBrowserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
//	LogDebug("sizeHint ~ w[%d] h[%d] ", option.rect.width(), option.rect.height());

	return QSize(ICON_ITEM_WIDTH, LIST_HEIGHT_MIN);
}

QWidget *ListBrowserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(parent)
	Q_UNUSED(option)
	Q_UNUSED(index)

//	ListBrowserEditor *editor = new ListBrowserEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
//	connect(editor, SIGNAL(SigClickPlay(int, CJsonNode)), this, SLOT(SlotClickPlay(int, CJsonNode)));
//	connect(editor, SIGNAL(SigClickTitle(int, CJsonNode)), this, SLOT(SlotClickTitle(int, CJsonNode)));
//	connect(editor, SIGNAL(SigMenu(int, int)), this, SLOT(SlotMenu(int, int)));
//	connect(editor, SIGNAL(SigMenu(int, int, QString)), this, SLOT(SlotMenu(int, int, QString)));
//	connect(editor, SIGNAL(SigMenuAction(CJsonNode, int, int)), this, SLOT(SlotMenuAction(CJsonNode, int, int)));
//	connect(editor, SIGNAL(SigMenuAction(QString, int, int)), this, SLOT(SlotMenuAction(QString, int, int)));

	return nullptr;
}

bool ListBrowserDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(ListBrowserDelegate::LIST_BROWSER_ID));
	int type = qvariant_cast<int>(index.data(ListBrowserDelegate::LIST_BROWSER_TYPE));
	QString cover = qvariant_cast<QString>(index.data(ListBrowserDelegate::LIST_BROWSER_COVER));
	QString rawData = qvariant_cast<QString>(index.data(ListBrowserDelegate::LIST_BROWSER_RAW));
	int nIndex = qvariant_cast<int>(index.data(ListBrowserDelegate::LIST_BROWSER_INDEX));

	QFont fontTitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmTitle(fontTitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	int indent = 0;
	if (SIDEMENU_ISERVICE == m_Service)
	{
		if (iAirableType_Mask_Sub & type)
		{
			indent = 40;
		}
	}
	QRect rectCover = QRect(rectBase.x() + 60 + indent, rectBase.y() + (rectBase.height() - 40) / 2, 40, 40);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 16 - 2, rectCover.y() + 2, 16, 16);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() + 15, rectBase.y() + (rectBase.height() - 16) / 2, 16, 16);
	QRect rectTitle = QRect(rectPlay.x() + rectPlay.width() + 15, rectBase.y() + (rectBase.height() - fmTitle.height()) / 2, 400, fmTitle.height());
	int gap = ( rectBase.width() - rectTitle.x() - (400 + 300 + 50 + 50 + 30 + 60) ) / 4;

	QRect rectSubtitle = QRect(rectTitle.x() + rectTitle.width() + gap, rectTitle.y(), 300, fmTitle.height());
	QRect rectDuration = QRect(rectSubtitle.x() + rectSubtitle.width() + gap, rectTitle.y(), 50, fmTitle.height());
	QRect rectFileSize = QRect(rectDuration.x() + rectDuration.width() + gap, rectTitle.y(), 50, fmTitle.height());
//	QRect rectMenu = QRect(rectFileSize.x() + rectFileSize.width() + gap, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);
	QRect rectMenu = QRect(rectBase.width() - 60 - 29, rectBase.y() + (rectBase.height() - 29) / 2, 29, 29);

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			CJsonNode node;
			if (!node.SetContent(rawData))
			{
				return false;
			}
			node.AddInt(KEY_ID_LOWER, id);
			node.AddString(KEY_ART, cover);

			if (rectCover.contains(curPoint))
			{
				emit SigSelectCheck(index);
			}
			else if (rectPlay.contains(curPoint))
			{
				emit SigSelectPlay(type, node);
			}
			else if (rectTitle.contains(curPoint))
			{
				emit SigSelectTitle(type, node);
			}
			else if (rectSubtitle.contains(curPoint))
			{
				emit SigSelectTitle(type, node);
			}
			else if (rectDuration.contains(curPoint))
			{
				emit SigSelectTitle(type, node);
			}
			else if (rectFileSize.contains(curPoint))
			{
				emit SigSelectTitle(type, node);
			}
			else if (rectMenu.contains(curPoint))
			{
				emit SigSelectMenu(index, curPoint);
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void ListBrowserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetService(m_Service);
//	widget->SetID(qvariant_cast<int>(index.data(LIST_BROWSER_ID)));
//	widget->SetType(qvariant_cast<int>(index.data(LIST_BROWSER_TYPE)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_BROWSER_COVER)));
//	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(LIST_BROWSER_INDEX)));
//	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_BROWSER_SELECT)));
//	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_BROWSER_TITLE)));
//	widget->SetSubtitle(qvariant_cast<QString>(index.data(LIST_BROWSER_SUBTITLE)));
//	widget->SetDuration(qvariant_cast<QString>(index.data(LIST_BROWSER_DURATION)));
//	widget->SetFileSize(qvariant_cast<QString>(index.data(LIST_BROWSER_FILESIZE)));
//	widget->SetRawData(qvariant_cast<QString>(index.data(LIST_BROWSER_RAW)));
//	widget->blockSignals(false);

//	widget->ClearMenu();
//	widget->SetMenu(m_OptionMenuMap);
//}

//void ListBrowserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
//	model->setData(index, widget->GetID(), LIST_BROWSER_ID);
//	model->setData(index, widget->GetType(), LIST_BROWSER_TYPE);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_BROWSER_COVER);
//	model->setData(index, widget->GetFormCoverArt()->GetIndex(), LIST_BROWSER_INDEX);
//	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_BROWSER_SELECT);
//	model->setData(index, widget->GetTitle(), LIST_BROWSER_TITLE);
//	model->setData(index, widget->GetSubtitle(), LIST_BROWSER_SUBTITLE);
//	model->setData(index, widget->GetDuration(), LIST_BROWSER_DURATION);
//	model->setData(index, widget->GetFilesize(), LIST_BROWSER_FILESIZE);
//	model->setData(index, widget->GetRawData(), LIST_BROWSER_RAW);
//}

//void ListBrowserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
