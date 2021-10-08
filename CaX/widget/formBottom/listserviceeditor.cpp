#include <QMouseEvent>

#include "listserviceeditor.h"
#include "ui_listserviceeditor.h"

#include "widget/form/formcoverart.h"

#include "util/log.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


ListServiceEditor::ListServiceEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	ui(new Ui::ListServiceEditor)
{
	ui->setupUi(this);

	m_nType = -1;
//	m_Icon.clear();
//	m_Art.clear();
	m_RawData.clear();

	ConnectSigToSlot();

}

ListServiceEditor::~ListServiceEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	delete ui;
}

QString ListServiceEditor::GetID() const
{
	return m_StrID;
}

void ListServiceEditor::SetID(QString strID)
{
	m_StrID = strID;
}

int ListServiceEditor::GetType() const
{
	return m_nType;
}

void ListServiceEditor::SetType(int nType)
{
	m_nType = nType;
}

QString ListServiceEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void ListServiceEditor::SetTitle(const QString &Title)
{
	ui->labelTitle->setText(Title);
}

QString ListServiceEditor::GetSubtitle() const
{
	return ui->labelSubtitle->text();
}

void ListServiceEditor::SetSubtitle(const QString &Bottom)
{
	ui->labelSubtitle->show();
	ui->labelSubtitle->setText(Bottom);
}

QString ListServiceEditor::GetDuration() const
{
	return ui->labelDuration->text();
}

void ListServiceEditor::SetDuration(const QString &Duration)
{
	ui->labelDuration->show();
	ui->labelDuration->setText(Duration);
}

QString ListServiceEditor::GetCoverArt() const
{
//	return m_Icon;
	return m_pFormCoverArt->GetCoverArt();
}

void ListServiceEditor::SetCoverArt(const QString &coverArt)
{
//	if (Icon.isEmpty() || !m_Art.isEmpty())
//		return;

//	m_Icon = Icon;

//	QString filename = "";
//	if (!m_Icon.compare(ICON_SEARCH))
//	{
//		filename = ":/resource/Icon-playbar-volume-160.png";
//	}
//	else if (!m_Icon.compare(ICON_REC_LIST))
//	{
//		filename = ":/resource/Icon-playbar-volume-160.png";
//	}
//	else
//	{
//		filename = ":/resource/Icon-playbar-volume-160.png";
//	}

//	QImage image;
//	if (image.load(filename))
//	{
////		painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelIcon->setPixmap(pixmap.scaled(ui->labelIcon->width()
//												   , ui->labelIcon->height()
//												   , Qt::KeepAspectRatio));
//	}

	m_pFormCoverArt->SetCoverArt(coverArt);

}

//QString ListServiceEditor::GetArt() const
//{
//	return m_Art;
//}

//void ListServiceEditor::SetArt(const QString &Art)
//{
//	m_Art = Art;

//	if (m_Art.isEmpty())
//	{
//		m_Art = ":/resource/Icon-playbar-volume-160.png";
//	}

//	QImage image;
//	if (image.load(m_Art))
//	{
////		painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelIcon->setPixmap(pixmap.scaled(ui->labelIcon->width()
//												   , ui->labelIcon->height()
//												   , Qt::KeepAspectRatio));
//	}

//}

QString ListServiceEditor::GetRawData() const
{
	return m_RawData;
}

void ListServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

bool ListServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
//		if (object == ui->labelIcon)
//		{
//			emit SigClickIcon(m_RawData);
//		}
//		else if (object == ui->labelTitle)
//		{
//			emit SigClickTitle(m_RawData);
//		}
	}

	return QObject::eventFilter(object, event);
}

void ListServiceEditor::SlotCoverArt()
{
	emit SigClickCoverArt(m_RawData);
}

void ListServiceEditor::ConnectSigToSlot()
{
//	ui->labelIcon->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->hide();
	ui->labelDuration->hide();

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));

}

