#include "iconservicedelegate.h"
#include "iconserviceeditor.h"

#include "iconservice.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/CJsonNode.h"
#include "util/log.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"


IconServiceDelegate::IconServiceDelegate()
{

}

void IconServiceDelegate::SlotClickPlay(int nType)
{
	emit SigSelectPlay(nType);
}

void IconServiceDelegate::SlotClickTitle(int nType, QString rawData)
{
	CJsonNode node(JSON_OBJECT);
	if (!node.SetContent(rawData))
	{
		return;
	}

	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (IconService::ICON_SERVICE_INPUT == m_nServiceType
			|| IconService::ICON_SERVICE_FM_RADIO == m_nServiceType
			|| IconService::ICON_SERVICE_DAB_RADIO == m_nServiceType
			|| (IconService::ICON_SERVICE_ISERVICE == m_nServiceType && node.GetString(KEY_ITEM_TYPE).isEmpty()))
	{
		emit SigSelectTitle(nType);
	}
	else
	{
		emit SigSelectTitle(nType, rawData);
	}

}


void IconServiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(painter)
	Q_UNUSED(option)
	Q_UNUSED(index)
}

QSize IconServiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	return QSize(ICON_ITEM_WIDTH - 35, ICON_ITEM_HEIGHT - 20);
}

QWidget *IconServiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	IconServiceEditor *editor = new IconServiceEditor(parent);
	connect(editor, SIGNAL(SigClickPlay(int)), this, SLOT(SlotClickPlay(int)));
	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));

	return editor;
}

void IconServiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
	widget->blockSignals(true);

	widget->SetID(qvariant_cast<int>(index.data(ICON_SERVICE_ID)));
	widget->SetType(qvariant_cast<int>(index.data(ICON_SERVICE_TYPE)));
	widget->SetRawData(qvariant_cast<QString>(index.data(ICON_SERVICE_RAW)));
	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_SERVICE_COVER)));
	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(ICON_SERVICE_TITLE)));
	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(ICON_SERVICE_SUBTITLE)));

	widget->blockSignals(false);
}

void IconServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
	model->setData(index, widget->GetID(), ICON_SERVICE_ID);
	model->setData(index, widget->GetType(), ICON_SERVICE_TYPE);
	model->setData(index, widget->GetRawData(), ICON_SERVICE_RAW);
	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), ICON_SERVICE_COVER);
	model->setData(index, widget->GetFormTitle()->GetTitle(), ICON_SERVICE_TITLE);
	model->setData(index, widget->GetFormTitle()->GetSubtitle(), ICON_SERVICE_SUBTITLE);
}

void IconServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

int IconServiceDelegate::GetServiceType() const
{
	return m_nServiceType;
}

void IconServiceDelegate::SetServiceType(int nService)
{
	m_nServiceType = nService;
}

QListView::ViewMode IconServiceDelegate::GetViewMode() const
{
	return m_ViewMode;
}

void IconServiceDelegate::SetViewMode(const QListView::ViewMode &ViewMode)
{
	m_ViewMode = ViewMode;
}
