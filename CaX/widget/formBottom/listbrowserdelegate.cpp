#include "listbrowserdelegate.h"
#include "listbrowsereditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"

ListBrowserDelegate::ListBrowserDelegate()
{
}

void ListBrowserDelegate::SlotClickPlay(int type, CJsonNode node)
{
	emit SigSelectPlay(type, node);
}

void ListBrowserDelegate::SlotClickTitle(int type, CJsonNode node)
{
	emit SigSelectTitle(type, node);
}

void ListBrowserDelegate::SlotMenu(int index, int type)
{
	emit SigMenu(index, type);
}

void ListBrowserDelegate::SlotMenu(int index, int type, QString menuName)
{
	emit SigMenu(index, type, menuName);
}

void ListBrowserDelegate::SlotMenuAction(QString path, int type, int menuID)
{
	emit SigMenuAction(path, type, menuID);
}

void ListBrowserDelegate::SlotMenuAction(CJsonNode node, int type, int menuID)
{
	emit SigMenuAction(node, type, menuID);
}

void ListBrowserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListBrowserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_BROWSER_HEIGHT);
}

QWidget *ListBrowserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListBrowserEditor *editor = new ListBrowserEditor(parent);
	connect(editor, SIGNAL(SigClickPlay(int, CJsonNode)), this, SLOT(SlotClickPlay(int, CJsonNode)));
	connect(editor, SIGNAL(SigClickTitle(int, CJsonNode)), this, SLOT(SlotClickTitle(int, CJsonNode)));
	connect(editor, SIGNAL(SigMenu(int, int)), this, SLOT(SlotMenu(int, int)));
	connect(editor, SIGNAL(SigMenu(int, int, QString)), this, SLOT(SlotMenu(int, int, QString)));
	connect(editor, SIGNAL(SigMenuAction(CJsonNode, int, int)), this, SLOT(SlotMenuAction(CJsonNode, int, int)));
	connect(editor, SIGNAL(SigMenuAction(QString, int, int)), this, SLOT(SlotMenuAction(QString, int, int)));

	return editor;
}

void ListBrowserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
	widget->blockSignals(true);
	widget->SetService(m_Service);
	widget->SetID(qvariant_cast<int>(index.data(LIST_BROWSER_ID)));
	widget->SetIndex(qvariant_cast<int>(index.data(LIST_BROWSER_INDEX)));
	widget->SetType(qvariant_cast<int>(index.data(LIST_BROWSER_TYPE)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(LIST_BROWSER_COVER)));
	widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(LIST_BROWSER_SELECT)));
	widget->SetTitle(qvariant_cast<QString>(index.data(LIST_BROWSER_TITLE)));
	widget->SetSubtitle(qvariant_cast<QString>(index.data(LIST_BROWSER_SUBTITLE)));
	widget->SetDuration(qvariant_cast<QString>(index.data(LIST_BROWSER_DURATION)));
	widget->SetFileSize(qvariant_cast<QString>(index.data(LIST_BROWSER_FILESIZE)));
	widget->SetRawData(qvariant_cast<QString>(index.data(LIST_BROWSER_RAW)));
	widget->blockSignals(false);

	widget->ClearMenu();
	widget->SetMenu(m_OptionMenuMap);
}

void ListBrowserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListBrowserEditor *widget = static_cast<ListBrowserEditor*>(editor);
	model->setData(index, widget->GetID(), LIST_BROWSER_ID);
	model->setData(index, widget->GetIndex(), LIST_BROWSER_INDEX);
	model->setData(index, widget->GetType(), LIST_BROWSER_TYPE);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), LIST_BROWSER_COVER);
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), LIST_BROWSER_SELECT);
	model->setData(index, widget->GetTitle(), LIST_BROWSER_TITLE);
	model->setData(index, widget->GetSubtitle(), LIST_BROWSER_SUBTITLE);
	model->setData(index, widget->GetDuration(), LIST_BROWSER_DURATION);
	model->setData(index, widget->GetFilesize(), LIST_BROWSER_FILESIZE);
	model->setData(index, widget->GetRawData(), LIST_BROWSER_RAW);
}

void ListBrowserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

int ListBrowserDelegate::GetService() const
{
	return m_Service;
}

void ListBrowserDelegate::SetService(int Service)
{
	m_Service = Service;
}

QMap<int, QString> ListBrowserDelegate::GetOptionMenuMap() const
{
	return m_OptionMenuMap;
}

void ListBrowserDelegate::SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap)
{
	m_OptionMenuMap = OptionMenuMap;
}

