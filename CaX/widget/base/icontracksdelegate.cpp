#include <QApplication>
#include <QPainter>
#include <QFile>

#include "icontracksdelegate.h"
#include "icontrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "network/tcpclient.h"

IconTracksDelegate::IconTracksDelegate()
{
	m_Image = QImage(":/resource/outline_arrow_forward_black_24dp.png");
}

void IconTracksDelegate::commitAndCloseEditor()
{
	IconTracksEditor *widget = static_cast<IconTracksEditor*>(sender());
	emit commitData(widget);
	emit closeEditor(widget);
}

void IconTracksDelegate::SlotClickCoverArt(int nID)
{
//	LogDebug("click cover art");
	emit SigSelectCoverArt(nID);

}

void IconTracksDelegate::SlotClickTop(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click title");
}

void IconTracksDelegate::SlotClickBottom(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click subtitle");
}

void IconTracksDelegate::SlotClickCount(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click count");
}

void IconTracksDelegate::SlotClickFavorite(int nID, int nFavorite)
{
	emit SigSelectFavorite(nID, nFavorite);
}

void IconTracksDelegate::SlotClickRating(int nID, int nRating)
{
	emit SigSelectRating(nID, nRating);

}

void IconTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)

//	QStyledItemDelegate::paint(painter, option, index);
//	painter->save();

//	int nID = qvariant_cast<int>(index.data(ICON_TRACKS_ID));
//	QString cover = qvariant_cast<QString>(index.data(ICON_TRACKS_COVER));
//	QString title = qvariant_cast<QString>(index.data(ICON_TRACKS_TITLE));
//	QString subtitle = qvariant_cast<QString>(index.data(ICON_TRACKS_SUBTITLE));
//	QString count = qvariant_cast<QString>(index.data(ICON_TRACKS_COUNT));

////	LogDebug("rect [%d] - [%d][%d][%d][%d]", nID, option.rect.top(), option.rect.bottom(), option.rect.left(), option.rect.right());

//	QRect coverRect = option.rect;
//	QRect titleRect = option.rect;
//	QRect subtitleRect = option.rect;
//	QRect countRect = option.rect;

//	if (m_ViewMode == QListView::ViewMode::ListMode)
//	{
//		coverRect.setWidth(coverRect.height());
//	}

//	bool bFoundImage = false;
//	if (QFile::exists(cover))
//	{
//		QImage image;
//		if (image.load(cover))
//		{
//			painter->drawImage(coverRect, image);
//			bFoundImage = true;
//		}
//	}

//	if (!bFoundImage)
//	{
//		painter->drawImage(coverRect, m_Image);
//	}

//	QFont font = QApplication::font();

//	painter->setFont(font);
//	painter->drawText(titleRect, title);
//	painter->drawText(subtitleRect, subtitle);
//	painter->drawText(countRect, count);

//	painter->restore();
}

QSize IconTracksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(CATEGORY_CONT_WIDTH, CATEGORY_CONT_HEIGHT + 50);
}

QWidget *IconTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	IconTracksEditor *editor = new IconTracksEditor(parent);
//	connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(editor, SIGNAL(SigClickTop(int)), this, SLOT(SlotClickTop(int)));
	connect(editor, SIGNAL(SigClickBottom(int)), this, SLOT(SlotClickBottom(int)));
	connect(editor, SIGNAL(SigClickCount(int)), this, SLOT(SlotClickCount(int)));
	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
	connect(editor, SIGNAL(SigClickRating(int, int)), this, SLOT(SlotClickRating(int, int)));

	return editor;
}

void IconTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(ICON_TRACKS_ID)));
	widget->SetCoverArt(qvariant_cast<QString>(index.data(ICON_TRACKS_COVER)));
	widget->SetTop(qvariant_cast<QString>(index.data(ICON_TRACKS_TOP)));
	widget->SetBottom(qvariant_cast<QString>(index.data(ICON_TRACKS_BOTTOM)));
	widget->SetCount(qvariant_cast<QString>(index.data(ICON_TRACKS_COUNT)));
	widget->SetFavorite(qvariant_cast<int>(index.data(ICON_TRACKS_FAVORITE)));
	widget->SetRating(qvariant_cast<int>(index.data(ICON_TRACKS_RATING)));
	widget->blockSignals(false);
}

void IconTracksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
	model->setData(index, widget->GetID(), ICON_TRACKS_ID);
	model->setData(index, widget->GetCoverArt(), ICON_TRACKS_COVER);
	model->setData(index, widget->GetTop(), ICON_TRACKS_TOP);
	model->setData(index, widget->GetBottom(), ICON_TRACKS_BOTTOM);
	model->setData(index, widget->GetCount(), ICON_TRACKS_COUNT);
	model->setData(index, widget->GetFavorite(), ICON_TRACKS_FAVORITE);
	model->setData(index, widget->GetRating(), ICON_TRACKS_RATING);
}

void IconTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}


QListView::ViewMode IconTracksDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void IconTracksDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}

