#include "searchtrackdelegate.h"
#include "searchtrackeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

SearchTrackDelegate::SearchTrackDelegate()
{

}

QMap<int, QString> SearchTrackDelegate::GetOptionMenuMap() const
{
	return m_OptionMenuMap;
}

void SearchTrackDelegate::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
{
	m_OptionMenuMap = OptionMenuMap;
}

void SearchTrackDelegate::SlotClickPlay(int nID)
{
	emit SigSelectPlay(nID, PLAY_CLEAR);
}

void SearchTrackDelegate::SlotMenuAction(int nID, int menuID, QString coverArt)
{
	emit SigMenuAction(nID, menuID, coverArt);
}

void SearchTrackDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(index);
}

QSize SearchTrackDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(LIST_ITEM_WIDTH, LIST_TRACKS_HEIGHT);
}

QWidget *SearchTrackDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	SearchTrackEditor *editor = new SearchTrackEditor(parent);
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigMenuAction(int, int, QString)), this, SLOT(SlotMenuAction(int, int, QString)));

	editor->ClearMenu();
	editor->SetMenu(m_OptionMenuMap);

	return editor;
}

void SearchTrackDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	SearchTrackEditor *widget = static_cast<SearchTrackEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(SEARCH_TRACKS_ID)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(SEARCH_TRACKS_COVER)));
	widget->SetTitle(qvariant_cast<QString>(index.data(SEARCH_TRACKS_TITLE)));
	widget->SetTime(qvariant_cast<QString>(index.data(SEARCH_TRACKS_TIME)));
	widget->SetArtist(qvariant_cast<QString>(index.data(SEARCH_TRACKS_ARTIST)));
	widget->SetAlbumID(qvariant_cast<int>(index.data(SEARCH_TRACKS_ALBUM_ID)));
	widget->SetAlbum(qvariant_cast<QString>(index.data(SEARCH_TRACKS_ALBUM)));
	widget->SetGenre(qvariant_cast<QString>(index.data(SEARCH_TRACKS_GENRE)));
	widget->blockSignals(false);
}

void SearchTrackDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	SearchTrackEditor *widget = static_cast<SearchTrackEditor*>(editor);
	model->setData(index, widget->GetID(), SEARCH_TRACKS_ID);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), SEARCH_TRACKS_COVER);
	model->setData(index, widget->GetTitle(), SEARCH_TRACKS_TITLE);
	model->setData(index, widget->GetTime(), SEARCH_TRACKS_TIME);
	model->setData(index, widget->GetArtist(), SEARCH_TRACKS_ARTIST);
	model->setData(index, widget->GetAlbumID(), SEARCH_TRACKS_ALBUM_ID);
	model->setData(index, widget->GetAlbum(), SEARCH_TRACKS_ALBUM);
	model->setData(index, widget->GetGenre(), SEARCH_TRACKS_GENRE);

}

void SearchTrackDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
