#include <QMouseEvent>

#include "iconserviceeditor.h"
#include "ui_iconserviceeditor.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"

#include "util/log.h"

IconServiceEditor::IconServiceEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	ui(new Ui::IconServiceEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_Type = -1;
//	m_CoverArt.clear();

	ConnectSigToSlot();
}

IconServiceEditor::~IconServiceEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	if (m_pFormTitle)
	{
		delete m_pFormTitle;
		m_pFormTitle = nullptr;
	}

	delete ui;
}

int IconServiceEditor::GetID()
{
	return m_ID;
}

void IconServiceEditor::SetID(int nID)
{
	m_ID = nID;
}

int IconServiceEditor::GetType() const
{
	return m_Type;
}

void IconServiceEditor::SetType(int Type)
{
	m_Type = Type;
}

QString IconServiceEditor::GetCoverArt()
{
//	return m_CoverArt;
	return m_pFormCoverArt->GetCoverArt();
}

void IconServiceEditor::SetCoverArt(const QString &coverArt)
{
//	m_CoverArt = value;

//	if (m_CoverArt.isEmpty())
//	{
//		m_CoverArt = ":/resource/Icon-playbar-volume-160.png";
//	}

//	QImage image;
//	if (image.load(m_CoverArt))
//	{
////		painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
//												   , ui->labelCoverArt->height()
//												   , Qt::KeepAspectRatio));
//	}
	m_pFormCoverArt->SetCoverArt(coverArt);
}

QString IconServiceEditor::GetTitle()
{
//	return ui->labelTitle->text();
	return m_pFormTitle->GetTitle();
}

void IconServiceEditor::SetTitle(const QString &title)
{
//	ui->labelTitle->setText(value);
	m_pFormTitle->SetTitle(title);
}

QString IconServiceEditor::GetSubtitle()
{
//	return ui->labelSubtitle->text();
	return m_pFormTitle->GetSubtitle();
}

void IconServiceEditor::SetSubtitle(const QString &subtitle)
{
//	ui->labelSubtitle->setText(value);
	m_pFormTitle->SetSubtitle(subtitle);
}

QString IconServiceEditor::GetRawData() const
{
	return m_RawData;
}

void IconServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

bool IconServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
//		if (object == ui->labelCoverArt)
//		{
//			emit SigClickCoverArt(m_Type, m_RawData);
//		}
//		else if (object == ui->labelTitle)
//		{
//			emit SigClickTitle(m_Type);
//		}
	}
	return QObject::eventFilter(object, event);
}

void IconServiceEditor::SlotCoverArt()
{
	emit SigClickCoverArt(m_Type, m_RawData);
}

void IconServiceEditor::SlotTitle()
{
	emit SigClickTitle(m_Type);
}

void IconServiceEditor::SlotSubtitle()
{
	LogDebug("good choice subtitle");
}

void IconServiceEditor::ConnectSigToSlot()
{
//	ui->labelCoverArt->installEventFilter(this);
//	ui->labelTitle->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}
