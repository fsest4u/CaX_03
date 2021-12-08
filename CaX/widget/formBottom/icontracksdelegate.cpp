#include "icontracksdelegate.h"
#include "icontrackseditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"

IconTracksDelegate::IconTracksDelegate()
{
	m_TypeMode = TYPE_MODE_MAX;
}

void IconTracksDelegate::SlotClickPlay(int nID)
{
	emit SigSelectPlay(nID, PLAY_CLEAR);
}

void IconTracksDelegate::SlotClickFavorite(int nID, int nFavorite)
{
	emit SigSelectFavorite(nID, nFavorite);
}

void IconTracksDelegate::SlotClickRating(int nID, int nRating)
{
	emit SigSelectRating(nID, nRating);

}

void IconTracksDelegate::SlotClickTitle(int nID, QString coverArt)
{
	emit SigSelectTitle(nID, coverArt);
}

void IconTracksDelegate::SlotClickSubtitle(int nID, QString coverArt)
{
	emit SigSelectSubtitle(nID, coverArt);
}

void IconTracksDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize IconTracksDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(ICON_ITEM_WIDTH - 35, ICON_ITEM_HEIGHT - 20);
}

QWidget *IconTracksDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	IconTracksEditor *editor = new IconTracksEditor(parent);
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigClickFavorite(int, int)), this, SLOT(SlotClickFavorite(int, int)));
	connect(editor, SIGNAL(SigClickRating(int, int)), this, SLOT(SlotClickRating(int, int)));
	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));
	connect(editor, SIGNAL(SigClickSubtitle(int, QString)), this, SLOT(SlotClickSubtitle(int, QString)));

	return editor;
}

void IconTracksDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(ICON_TRACKS_ID)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_TRACKS_COVER)));
	widget->GetFormCoverArt()->SetCount(qvariant_cast<int>(index.data(ICON_TRACKS_COUNT)));
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		widget->GetFormCoverArt()->SetFavorite(qvariant_cast<int>(index.data(ICON_TRACKS_FAVORITE)));
		widget->GetFormCoverArt()->SetRating(qvariant_cast<int>(index.data(ICON_TRACKS_RATING)));
	}
	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(ICON_TRACKS_SELECT)));
	widget->GetFormTitle()->SetTitleFont(FONT_SIZE_ICON_TITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_BOLD);
	widget->GetFormTitle()->SetSubtitleFont(FONT_SIZE_ICON_SUBTITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(ICON_TRACKS_TITLE)));
	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(ICON_TRACKS_SUBTITLE)));
	widget->blockSignals(false);
}

void IconTracksDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
	model->setData(index, widget->GetID(), ICON_TRACKS_ID);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), ICON_TRACKS_COVER);
	model->setData(index, widget->GetFormCoverArt()->GetCount(), ICON_TRACKS_COUNT);
	if (m_TypeMode == TYPE_MODE_ITEM)
	{
		model->setData(index, widget->GetFormCoverArt()->GetFavorite(), ICON_TRACKS_FAVORITE);
		model->setData(index, widget->GetFormCoverArt()->GetRating(), ICON_TRACKS_RATING);
	}
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), ICON_TRACKS_SELECT);
	model->setData(index, widget->GetFormTitle()->GetTitle(), ICON_TRACKS_TITLE);
	model->setData(index, widget->GetFormTitle()->GetSubtitle(), ICON_TRACKS_SUBTITLE);
}

void IconTracksDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

	QRect rect = option.rect;
	rect.setWidth(m_Resize);
	rect.setHeight(m_Resize * 1.25);

	IconTracksEditor *widget = static_cast<IconTracksEditor*>(editor);
	widget->setGeometry(rect);
}

int IconTracksDelegate::GetTypeMode() const
{
	return m_TypeMode;
}

void IconTracksDelegate::SetTypeMode(int TypeMode)
{
	m_TypeMode = TypeMode;
}

int IconTracksDelegate::GetResize() const
{
	return m_Resize;
}

void IconTracksDelegate::SetResize(int Resize)
{
	m_Resize = Resize;
}
