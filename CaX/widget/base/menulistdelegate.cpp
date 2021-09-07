#include "menulistdelegate.h"
#include "menulisteditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

MenuListDelegate::MenuListDelegate()
{

}

QListView::ViewMode MenuListDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void MenuListDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}

void MenuListDelegate::SlotClickIcon(QString rawData)
{
	emit SigSelectIcon(rawData);
}

void MenuListDelegate::SlotClickTitle(QString rawData)
{
	emit SigSelectTitle(rawData);
}

void MenuListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize MenuListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(SONG_CONT_WIDTH, SONG_CONT_HEIGHT + 50);
}

QWidget *MenuListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	MenuListEditor *editor = new MenuListEditor(parent);
	connect(editor, SIGNAL(SigClickIcon(QString)), this, SLOT(SlotClickIcon(QString)));
	connect(editor, SIGNAL(SigClickTitle(QString)), this, SLOT(SlotClickTitle(QString)));

	return editor;
}

void MenuListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//	LogDebug("type : [%d]", qvariant_cast<int>(index.data(MENU_LIST_ROLE_TYPE)));
//	LogDebug("icon : [%s]", qvariant_cast<QString>(index.data(MENU_LIST_ROLE_ICON)).toUtf8().data());
//	LogDebug("title : [%s]", qvariant_cast<QString>(index.data(MENU_LIST_ROLE_TITLE)).toUtf8().data());
//	LogDebug("url : [%s]", qvariant_cast<QString>(index.data(MENU_LIST_ROLE_URL)).toUtf8().data());
	MenuListEditor *widget = static_cast<MenuListEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<QString>(index.data(MENU_LIST_ROLE_ID)));
	widget->SetType(qvariant_cast<int>(index.data(MENU_LIST_ROLE_TYPE)));
	widget->SetTitle(qvariant_cast<QString>(index.data(MENU_LIST_ROLE_TITLE)));
	widget->SetIcon(qvariant_cast<QString>(index.data(MENU_LIST_ROLE_ICON)));
	widget->SetArt(qvariant_cast<QString>(index.data(MENU_LIST_ROLE_ART)));
	widget->SetRawData(qvariant_cast<QString>(index.data(MENU_LIST_ROLE_RAW)));
	widget->blockSignals(false);
}

void MenuListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	MenuListEditor *widget = static_cast<MenuListEditor*>(editor);
	model->setData(index, widget->GetID(), MENU_LIST_ROLE_ID);
	model->setData(index, widget->GetType(), MENU_LIST_ROLE_TYPE);
	model->setData(index, widget->GetTitle(), MENU_LIST_ROLE_TITLE);
	model->setData(index, widget->GetIcon(), MENU_LIST_ROLE_ICON);
	model->setData(index, widget->GetArt(), MENU_LIST_ROLE_ART);
	model->setData(index, widget->GetRawData(), MENU_LIST_ROLE_RAW);
}

void MenuListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
