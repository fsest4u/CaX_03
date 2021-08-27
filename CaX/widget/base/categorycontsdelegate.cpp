#include <QApplication>
#include <QPainter>
#include <QFile>

#include "categorycontsdelegate.h"
#include "categorycontseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "network/tcpclient.h"

CategoryContsDelegate::CategoryContsDelegate()
{
	m_Image = QImage(":/resource/outline_arrow_forward_black_24dp.png");
}

void CategoryContsDelegate::commitAndCloseEditor()
{
	CategoryContsEditor *widget = static_cast<CategoryContsEditor*>(sender());
	emit commitData(widget);
	emit closeEditor(widget);
}

void CategoryContsDelegate::SlotClickCoverArt(int nID)
{
//	LogDebug("click cover art");
	emit SigSelectCoverArt(nID);

}

void CategoryContsDelegate::SlotClickTitle(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click title");
}

void CategoryContsDelegate::SlotClickSubtitle(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click subtitle");
}

void CategoryContsDelegate::SlotClickCount(int nID)
{
	Q_UNUSED(nID)
	LogDebug("click count");
}

void CategoryContsDelegate::SlotClickFavorite(int nID, int nFavorite)
{
	emit SigSelectFavorite(nID, nFavorite);
}

void CategoryContsDelegate::SlotClickRating(int nID, int nRating)
{
	emit SigSelectRating(nID, nRating);

}

void CategoryContsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)

//	QStyledItemDelegate::paint(painter, option, index);
//	painter->save();

//	int nID = qvariant_cast<int>(index.data(CATEGORY_ROLE_ID));
//	QString cover = qvariant_cast<QString>(index.data(CATEGORY_ROLE_COVER));
//	QString title = qvariant_cast<QString>(index.data(CATEGORY_ROLE_TITLE));
//	QString subtitle = qvariant_cast<QString>(index.data(CATEGORY_ROLE_SUBTITLE));
//	QString count = qvariant_cast<QString>(index.data(CATEGORY_ROLE_COUNT));

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

QSize CategoryContsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(CATEGORY_CONT_WIDTH, CATEGORY_CONT_HEIGHT + 50);
}

QWidget *CategoryContsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	CategoryContsEditor *editor = new CategoryContsEditor(parent);
//	connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));
	connect(editor, SIGNAL(SigClickSubtitle(int)), this, SLOT(SlotClickSubtitle(int)));
	connect(editor, SIGNAL(SigClickCount(int)), this, SLOT(SlotClickCount(int)));
	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
	connect(editor, SIGNAL(SigClickRating(int, int)), this, SLOT(SlotClickRating(int, int)));

	return editor;
}

void CategoryContsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	CategoryContsEditor *widget = static_cast<CategoryContsEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(CATEGORY_ROLE_ID)));
	widget->SetCoverArt(qvariant_cast<QString>(index.data(CATEGORY_ROLE_COVER)));
	widget->SetTitle(qvariant_cast<QString>(index.data(CATEGORY_ROLE_TITLE)));
	widget->SetSubtitle(qvariant_cast<QString>(index.data(CATEGORY_ROLE_SUBTITLE)));
	widget->SetCount(qvariant_cast<QString>(index.data(CATEGORY_ROLE_COUNT)));
	widget->SetFavorite(qvariant_cast<int>(index.data(CATEGORY_ROLE_FAVORITE)));
	widget->SetRating(qvariant_cast<int>(index.data(CATEGORY_ROLE_RATING)));
	widget->blockSignals(false);
}

void CategoryContsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	CategoryContsEditor *widget = static_cast<CategoryContsEditor*>(editor);
	model->setData(index, widget->GetID(), CATEGORY_ROLE_ID);
	model->setData(index, widget->GetCoverArt(), CATEGORY_ROLE_COVER);
	model->setData(index, widget->GetTitle(), CATEGORY_ROLE_TITLE);
	model->setData(index, widget->GetSubtitle(), CATEGORY_ROLE_SUBTITLE);
	model->setData(index, widget->GetCount(), CATEGORY_ROLE_COUNT);
	model->setData(index, widget->GetFavorite(), CATEGORY_ROLE_FAVORITE);
	model->setData(index, widget->GetRating(), CATEGORY_ROLE_RATING);
}

void CategoryContsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}


QListView::ViewMode CategoryContsDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void CategoryContsDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}

