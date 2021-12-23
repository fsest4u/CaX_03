#include "listsetupdelegate.h"
#include "listsetupeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

ListSetupDelegate::ListSetupDelegate()
{
	m_MenuMap.clear();
}

QMap<QString, CJsonNode> ListSetupDelegate::GetMenuMap() const
{
	return m_MenuMap;
}

void ListSetupDelegate::SetMenuMap(const QMap<QString, CJsonNode> &MenuMap)
{
	m_MenuMap = MenuMap;
}

void ListSetupDelegate::SlotClickTitle(QString strID, int index)
{
	emit SigSelectTitle(strID, index);
}

void ListSetupDelegate::SlotMenuAction(QString strID, QString json)
{
	emit SigMenuAction(strID, json);
}

void ListSetupDelegate::SlotSubMenuAction(QString strID, QString json)
{
	emit SigSubMenuAction(strID, json);
}

void ListSetupDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListSetupDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *ListSetupDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListSetupEditor *editor = new ListSetupEditor(parent);
	connect(editor, SIGNAL(SigClickTitle(QString, int)), this, SLOT(SlotClickTitle(QString, int)));
	connect(editor, SIGNAL(SigMenuAction(QString, QString)), this, SLOT(SlotMenuAction(QString, QString)));
	connect(editor, SIGNAL(SigSubMenuAction(QString, QString)), this, SLOT(SlotSubMenuAction(QString, QString)));

	return editor;
}

void ListSetupDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
	widget->blockSignals(true);
	widget->SetStrID(qvariant_cast<QString>(index.data(LIST_SETUP_ID)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_SETUP_TITLE)));
	widget->SetIndex(qvariant_cast<int>(index.data(LIST_SETUP_INDEX)));
	widget->blockSignals(false);

	widget->ClearMenu();
	widget->SetMenu(m_MenuMap);
}

void ListSetupDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListSetupEditor *widget = static_cast<ListSetupEditor*>(editor);
	model->setData(index, widget->GetStrID(), LIST_SETUP_ID);
	model->setData(index, widget->GetTitle(), LIST_SETUP_TITLE);
	model->setData(index, widget->GetIndex(), LIST_SETUP_INDEX);
}

void ListSetupDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
