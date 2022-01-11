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

void IconServiceDelegate::SlotClickCoverArt(int index)
{
	emit SigSelectCoverArt(index);
}

void IconServiceDelegate::SlotClickPlay(int index, bool muted)
{
	emit SigSelectPlay(index, muted);
}

void IconServiceDelegate::SlotClickTitle(int nType, QString rawData)
{
	CJsonNode node(JSON_OBJECT);
	if (!node.SetContent(rawData))
	{
		return;
	}

	LogDebug("node [%s]", node.ToCompactByteArray().data());

	if (IconService::ICON_SERVICE_INPUT == m_Service
			|| IconService::ICON_SERVICE_FM_RADIO == m_Service
			|| IconService::ICON_SERVICE_DAB_RADIO == m_Service
			|| (IconService::ICON_SERVICE_ISERVICE == m_Service && node.GetString(KEY_ITEM_TYPE).isEmpty()))
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
	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
	connect(editor, SIGNAL(SigClickPlay(int, bool)), this, SLOT(SlotClickPlay(int, bool)));
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
	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(ICON_SERVICE_INDEX)));
	if (IconService::ICON_SERVICE_DAB_RADIO == m_Service
			|| IconService::ICON_SERVICE_FM_RADIO == m_Service
			|| IconService::ICON_SERVICE_ISERVICE == m_Service
			|| IconService::ICON_SERVICE_BROWSER == m_Service)
	{
		widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(ICON_SERVICE_SELECT)));
	}
	if (IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
	{
		widget->GetFormCoverArt()->SetMute(qvariant_cast<bool>(index.data(ICON_SERVICE_MUTE)));
	}
	widget->GetFormTitle()->SetTitleFont(FONT_SIZE_ICON_TITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_BOLD);
	widget->GetFormTitle()->SetSubtitleFont(FONT_SIZE_ICON_SUBTITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
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
	model->setData(index, widget->GetFormCoverArt()->GetIndex(), ICON_SERVICE_INDEX);
	model->setData(index, widget->GetFormCoverArt()->GetSelect(), ICON_SERVICE_SELECT);
	if (IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
	{
		model->setData(index, widget->GetFormCoverArt()->GetMute(), ICON_SERVICE_MUTE);
	}
	model->setData(index, widget->GetFormTitle()->GetTitle(), ICON_SERVICE_TITLE);
	model->setData(index, widget->GetFormTitle()->GetSubtitle(), ICON_SERVICE_SUBTITLE);
}

void IconServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
}

int IconServiceDelegate::GetService() const
{
	return m_Service;
}

void IconServiceDelegate::SetService(int Service)
{
	m_Service = Service;
}
