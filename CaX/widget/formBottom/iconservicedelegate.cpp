#include <QPainter>
#include <QMouseEvent>

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

int IconServiceDelegate::GetService() const
{
	return m_Service;
}

void IconServiceDelegate::SetService(int Service)
{
	m_Service = Service;
}

//void IconServiceDelegate::SlotClickCoverArt(int index)
//{
//	emit SigSelectCoverArt(index);
//}

//void IconServiceDelegate::SlotClickPlay(int index, bool muted)
//{
//	emit SigSelectPlay(index, muted);
//}

//void IconServiceDelegate::SlotClickTitle(int nType, QString rawData)
//{
//	CJsonNode node(JSON_OBJECT);
//	if (!node.SetContent(rawData))
//	{
//		return;
//	}

//	LogDebug("node [%s]", node.ToCompactByteArray().data());

//	if (IconService::ICON_SERVICE_INPUT == m_Service
//			|| IconService::ICON_SERVICE_FM_RADIO == m_Service
//			|| IconService::ICON_SERVICE_DAB_RADIO == m_Service
//			|| (IconService::ICON_SERVICE_ISERVICE == m_Service && node.GetString(KEY_ITEM_TYPE).isEmpty()))
//	{
//		emit SigSelectTitle(nType);
//	}
//	else
//	{
//		emit SigSelectTitle(nType, rawData);
//	}

//}


void IconServiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->save();

	QString cover = qvariant_cast<QString>(index.data(ICON_SERVICE_COVER));
//	QString count = qvariant_cast<QString>(index.data(ICON_TRACKS_COUNT));
	QString title = qvariant_cast<QString>(index.data(ICON_SERVICE_TITLE));
	QString subtitle = qvariant_cast<QString>(index.data(ICON_SERVICE_SUBTITLE));
	bool select = qvariant_cast<bool>(index.data(ICON_SERVICE_SELECT));
	bool mute = qvariant_cast<bool>(index.data(ICON_SERVICE_MUTE));

	QFont fontCount("Segoe UI", 12, QFont::Normal, false);
	QFont fontTitle("Segoe UI", 14, QFont::Bold, false);
	QFont fontSubtitle("Segoe UI", 14, QFont::Normal, false);
	QFontMetrics fmCount(fontCount);
	QFontMetrics fmTitle(fontTitle);
	QFontMetrics fmSubtitle(fontSubtitle);

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 30 - 2, rectCover.y() + 2, 30, 30);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() - 30 -2, rectCover.y() + rectCover.height() - 30 - 2, 30, 30);
//	QRect rectCount = QRect(rectPlay.x() + (rectPlay.width() - fmCount.width(count)) / 2, rectPlay.y() + (rectPlay.height() - fmCount.height()) / 2, fmCount.width(count), fmCount.height());
	QRect rectTitle = QRect(rectBase.x(), rectBase.y() + rectCover.height(), rectCover.width(), fmTitle.height());
	QRect rectSubtitle = QRect(rectBase.x(), rectTitle.y() + fmTitle.height(), rectCover.width(), fmSubtitle.height());

//	LogDebug("orig x [%d] y [%d] w[%d] h[%d] row [%d]", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());
//	painter->drawRect(rectOrig);
//	painter->drawRect(rectCover);
//	painter->drawRect(rectCheck);
//	painter->drawRect(rectPlay);
//	painter->drawRect(rectTitle);
//	painter->drawRect(rectSubtitle);

	QPixmap pixCover;
	if (!cover.isEmpty() && pixCover.load(cover))
	{
		painter->drawPixmap(rectCover, pixCover);
	}


	if (IconService::ICON_SERVICE_FM_RADIO == m_Service
			|| IconService::ICON_SERVICE_DAB_RADIO == m_Service
			|| IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
	{
		QPixmap pixCheck;
		QString resCheck;
		if (select)
		{
			resCheck = QString(":/resource/playlist-btn30-selecton-h@2x.png");
		}
		else
		{
			resCheck = QString(":/resource/playlist-btn30-selecton-n@2x.png");
		}
		if (pixCheck.load(resCheck))
		{
			painter->drawPixmap(rectCheck, pixCheck);
		}
	}

	if (IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
	{
		QPixmap pixPlay;
		QString resPlay;
		if (mute)
		{
			resPlay = QString(":/resource/groupp-icon30-volume-mute@2x.png");
		}
		else
		{
			resPlay = QString(":/resource/groupp-icon30-volume@2x.png");
		}
		if (pixPlay.load(resPlay))
		{
			painter->drawPixmap(rectPlay, pixPlay);
		}
	}

	painter->setPen(QColor(84, 84, 84));
	if (!title.isEmpty())
	{
		painter->setFont(fontTitle);
		painter->drawText(rectTitle, title);
	}
	if (!subtitle.isEmpty())
	{
		painter->setFont(fontSubtitle);
		painter->drawText(rectSubtitle, subtitle);
	}

	painter->restore();
}

QSize IconServiceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

//	LogDebug("sizeHint ~");
	return QSize(option.rect.width(), option.rect.height());
}

QWidget *IconServiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

//	IconServiceEditor *editor = new IconServiceEditor(parent);
//	connect(editor, SIGNAL(SigClickCoverArt(int)), this, SLOT(SlotClickCoverArt(int)));
//	connect(editor, SIGNAL(SigClickPlay(int, bool)), this, SLOT(SlotClickPlay(int, bool)));
//	connect(editor, SIGNAL(SigClickTitle(int, QString)), this, SLOT(SlotClickTitle(int, QString)));

	return nullptr;
}

bool IconServiceDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	int id = qvariant_cast<int>(index.data(ICON_SERVICE_ID));
	int type = qvariant_cast<int>(index.data(ICON_SERVICE_TYPE));
	bool mute = qvariant_cast<bool>(index.data(ICON_SERVICE_MUTE));
	QString rawData = qvariant_cast<QString>(index.data(ICON_SERVICE_RAW));

	QRect rectOrig = option.rect;
	QRect rectBase = QRect(rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height());
	QRect rectCover = QRect(rectBase.x(), rectBase.y(), rectBase.width() * 0.9, rectBase.width() * 0.9);
	QRect rectCheck = QRect(rectCover.x() + rectCover.width() - 30 - 2, rectCover.y() + 2, 30, 30);
	QRect rectPlay = QRect(rectCover.x() + rectCover.width() - 30 -2, rectCover.y() + rectCover.height() - 30 - 2, 30, 30);

	QPoint curPoint(((QMouseEvent*)event)->x(), ((QMouseEvent*)event)->y());
//	LogDebug("editorEvent ~ x [%d] y [%d] w [%d] h [%d] row [%d] ", rectOrig.x(), rectOrig.y(), rectOrig.width(), rectOrig.height(), index.row());

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (rectCheck.contains(curPoint))
			{
				emit SigSelectCheck(index);
			}
			else if (rectPlay.contains(curPoint))
			{
				emit SigSelectPlay(id, mute);
			}
			else if (rectCover.contains(curPoint))
			{
				CJsonNode node(JSON_OBJECT);
				if (!node.SetContent(rawData))
				{
					return false;
				}

				if (IconService::ICON_SERVICE_INPUT == m_Service
						|| IconService::ICON_SERVICE_FM_RADIO == m_Service
						|| IconService::ICON_SERVICE_DAB_RADIO == m_Service
						|| (IconService::ICON_SERVICE_ISERVICE == m_Service && node.GetString(KEY_ITEM_TYPE).isEmpty()))
				{
					emit SigSelectTitle(type);
				}
				else
				{
					emit SigSelectTitle(type, rawData);
				}
			}
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//void IconServiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
//	widget->blockSignals(true);
//	widget->SetID(qvariant_cast<int>(index.data(ICON_SERVICE_ID)));
//	widget->SetType(qvariant_cast<int>(index.data(ICON_SERVICE_TYPE)));
//	widget->SetRawData(qvariant_cast<QString>(index.data(ICON_SERVICE_RAW)));
//	widget->GetFormCoverArt()->SetCoverArt(qvariant_cast<QString>(index.data(ICON_SERVICE_COVER)));
//	widget->GetFormCoverArt()->SetIndex(qvariant_cast<int>(index.data(ICON_SERVICE_INDEX)));
//	if (IconService::ICON_SERVICE_DAB_RADIO == m_Service
//			|| IconService::ICON_SERVICE_FM_RADIO == m_Service
//			|| IconService::ICON_SERVICE_ISERVICE == m_Service
//			|| IconService::ICON_SERVICE_BROWSER == m_Service)
//	{
//		widget->GetFormCoverArt()->SetSelect(qvariant_cast<bool>(index.data(ICON_SERVICE_SELECT)));
//	}
//	if (IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
//	{
//		widget->GetFormCoverArt()->SetMute(qvariant_cast<bool>(index.data(ICON_SERVICE_MUTE)));
//	}
//	widget->GetFormTitle()->SetTitleFont(FONT_SIZE_ICON_TITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_BOLD);
//	widget->GetFormTitle()->SetSubtitleFont(FONT_SIZE_ICON_SUBTITLE, FONT_COLOR_NORMAL, FONT_WEIGHT_NORMAL);
//	widget->GetFormTitle()->SetTitle(qvariant_cast<QString>(index.data(ICON_SERVICE_TITLE)));
//	widget->GetFormTitle()->SetSubtitle(qvariant_cast<QString>(index.data(ICON_SERVICE_SUBTITLE)));
//	widget->blockSignals(false);
//}

//void IconServiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//	IconServiceEditor *widget = static_cast<IconServiceEditor*>(editor);
//	model->setData(index, widget->GetID(), ICON_SERVICE_ID);
//	model->setData(index, widget->GetType(), ICON_SERVICE_TYPE);
//	model->setData(index, widget->GetRawData(), ICON_SERVICE_RAW);
//	model->setData(index, widget->GetFormCoverArt()->GetCoverArt(), ICON_SERVICE_COVER);
//	model->setData(index, widget->GetFormCoverArt()->GetIndex(), ICON_SERVICE_INDEX);
//	model->setData(index, widget->GetFormCoverArt()->GetSelect(), ICON_SERVICE_SELECT);
//	if (IconService::ICON_SERVICE_GROUP_PLAY == m_Service)
//	{
//		model->setData(index, widget->GetFormCoverArt()->GetMute(), ICON_SERVICE_MUTE);
//	}
//	model->setData(index, widget->GetFormTitle()->GetTitle(), ICON_SERVICE_TITLE);
//	model->setData(index, widget->GetFormTitle()->GetSubtitle(), ICON_SERVICE_SUBTITLE);
//}

//void IconServiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	Q_UNUSED(index)
//	editor->setGeometry(option.rect);
//}
