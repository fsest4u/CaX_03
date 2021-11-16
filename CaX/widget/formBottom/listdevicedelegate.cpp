#include "listdevicedelegate.h"
#include "listdeviceeditor.h"

#include "util/caxconstants.h"
#include "util/log.h"

ListDeviceDelegate::ListDeviceDelegate()
{

}

void ListDeviceDelegate::SlotClickDevice(QString mac)
{
	emit SigSelectDevice(mac);
}

void ListDeviceDelegate::SlotClickCancel(QString mac)
{
	emit SigSelectCancel(mac);
}

void ListDeviceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize ListDeviceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(LIST_ITEM_WIDTH, LIST_DEVICE_HEIGHT);
}

QWidget *ListDeviceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	ListDeviceEditor *editor = new ListDeviceEditor(parent);
	connect(editor, SIGNAL(SigClickDevice(QString)), this, SLOT(SlotClickDevice(QString)));
	connect(editor, SIGNAL(SigClickCancel(QString)), this, SLOT(SlotClickCancel(QString)));

	editor->SetLabelCancelShow(true);

	return editor;
}

void ListDeviceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ListDeviceEditor *widget = static_cast<ListDeviceEditor*>(editor);
	widget->blockSignals(true);
	widget->SetMac(qvariant_cast<QString>(index.data(LIST_DEVICE_MAC)));
	widget->SetAddr(qvariant_cast<QString>(index.data(LIST_DEVICE_ADDR)));
	widget->SetVal(qvariant_cast<QString>(index.data(LIST_DEVICE_VAL)));
	widget->SetDev(qvariant_cast<QString>(index.data(LIST_DEVICE_DEV)));
	widget->blockSignals(false);
}

void ListDeviceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ListDeviceEditor *widget = static_cast<ListDeviceEditor*>(editor);
	model->setData(index, widget->GetMac(), LIST_DEVICE_MAC);
	model->setData(index, widget->GetAddr(), LIST_DEVICE_ADDR);
	model->setData(index, widget->GetVal(), LIST_DEVICE_VAL);
	model->setData(index, widget->GetDev(), LIST_DEVICE_DEV);
}

void ListDeviceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}
