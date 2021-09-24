#include "menuicondelegate.h"
#include "menuiconeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"
#include "util/CJsonNode.h"

MenuIconDelegate::MenuIconDelegate()
{

}

void MenuIconDelegate::SlotClickCoverArt(int nType, QString rawData)
{
	LogDebug("click cover art");
	CJsonNode node(JSON_OBJECT);
	if (!node.SetContent(rawData))
	{
		return;
	}

//	if (node.GetString("ItemType").isEmpty())
//	{
//		emit SigSelectCoverArt(nType);

//	}
//	else
	{
		// for i-service
		emit SigSelectCoverArt(nType, rawData);
	}

}

void MenuIconDelegate::SlotClickTitle(int nType)
{
	Q_UNUSED(nType)
	LogDebug("click title");
}

void MenuIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize MenuIconDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(CATEGORY_CONT_WIDTH, CATEGORY_CONT_HEIGHT + 50);
}

QWidget *MenuIconDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	MenuIconEditor *editor = new MenuIconEditor(parent);
	connect(editor, SIGNAL(SigClickCoverArt(int, QString)), this, SLOT(SlotClickCoverArt(int, QString)));
	connect(editor, SIGNAL(SigClickTitle(int)), this, SLOT(SlotClickTitle(int)));

	return editor;
}

void MenuIconDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	MenuIconEditor *widget = static_cast<MenuIconEditor*>(editor);
	widget->blockSignals(true);
	widget->SetID(qvariant_cast<int>(index.data(MENU_ICON_ROLE_ID)));
	widget->SetType(qvariant_cast<int>(index.data(MENU_ICON_ROLE_TYPE)));
	widget->SetCoverArt(qvariant_cast<QString>(index.data(MENU_ICON_ROLE_COVER)));
	widget->SetTitle(qvariant_cast<QString>(index.data(MENU_ICON_ROLE_TITLE)));
	widget->SetRawData(qvariant_cast<QString>(index.data(MENU_ICON_ROLE_RAW)));
	widget->blockSignals(false);
}

void MenuIconDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	MenuIconEditor *widget = static_cast<MenuIconEditor*>(editor);
	model->setData(index, widget->GetID(), MENU_ICON_ROLE_ID);
	model->setData(index, widget->GetType(), MENU_ICON_ROLE_TYPE);
	model->setData(index, widget->GetCoverArt(), MENU_ICON_ROLE_COVER);
	model->setData(index, widget->GetTitle(), MENU_ICON_ROLE_TITLE);
	model->setData(index, widget->GetRawData(), MENU_ICON_ROLE_RAW);
}

void MenuIconDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

QListView::ViewMode MenuIconDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void MenuIconDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}
