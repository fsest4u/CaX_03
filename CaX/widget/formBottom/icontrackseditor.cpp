#include <QMouseEvent>

#include "icontrackseditor.h"
#include "ui_icontrackseditor.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"

#include "util/log.h"

IconTracksEditor::IconTracksEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	ui(new Ui::IconTracksEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_CoverArt.clear();

	ConnectSigToSlot();

}

IconTracksEditor::~IconTracksEditor()
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

int IconTracksEditor::GetID()
{
	return m_ID;
}

void IconTracksEditor::SetID(int nID)
{
	m_ID = nID;
}

QString IconTracksEditor::GetCoverArt()
{
//	return m_CoverArt;
	return m_pFormCoverArt->GetCoverArt();
}

void IconTracksEditor::SetCoverArt(const QString &coverArt)
{
	m_CoverArt = coverArt;

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

QString IconTracksEditor::GetTitle()
{
//	return ui->labelTop->text();
	return m_pFormTitle->GetTitle();
}

void IconTracksEditor::SetTitle(const QString &title)
{
//	ui->labelTop->setText(value);
	m_pFormTitle->SetTitle(title);
}

QString IconTracksEditor::GetSubtitle()
{
//	return ui->labelBottom->text();
	return m_pFormTitle->GetSubtitle();
}

void IconTracksEditor::SetSubtitle(const QString &subtitle)
{
//	ui->labelBottom->setText(value);
	m_pFormTitle->SetSubtitle(subtitle);
}

//QString IconTracksEditor::GetCount()
//{
//	return ui->labelCount->text();

//}

//void IconTracksEditor::SetCount(const QString &value)
//{
//	if (value.isEmpty())
//	{
//		ui->labelCount->hide();
//	}

//	ui->labelCount->setText(value);
//}


//int IconTracksEditor::GetRating() const
//{
//	return ui->labelRating->text().toInt();
//}

//void IconTracksEditor::SetRating(const int &value)
//{
//	if (value < 0)
//	{
//		ui->labelRating->hide();
//	}

//	ui->labelRating->setText(QString("%1").arg(value));
//}

//int IconTracksEditor::GetFavorite() const
//{
//	return ui->labelFavorite->text().toInt();
//}

//void IconTracksEditor::SetFavorite(const int &value)
//{
//	if (value < 0)
//	{
//		ui->labelFavorite->hide();
//	}

//	ui->labelFavorite->setText(QString("%1").arg(value));
//}

bool IconTracksEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
//		if (object == ui->labelCoverArt)
//		{
//			emit SigClickCoverArt(m_ID, m_CoverArt);
//		}
//		else if (object == ui->labelTop)
//		{
//			emit SigClickTop(m_ID);
//		}
//		else if (object == ui->labelBottom)
//		{
//			emit SigClickBottom(m_ID);
//		}
//		else if (object == ui->labelCount)
//		{
//			emit SigClickCount(m_ID);
//		}
//		else if (object == ui->labelFavorite)
//		{
//			emit SigClickFavorite(m_ID, 1);
//		}
//		else if (object == ui->labelRating)
//		{
//			emit SigClickRating(m_ID, 5);
//		}
	}
	return QObject::eventFilter(object, event);
}

void IconTracksEditor::SlotCoverArt()
{
	emit SigClickCoverArt(m_ID, m_CoverArt);
}

void IconTracksEditor::SlotTitle()
{
	emit SigClickTitle(m_ID);
}

void IconTracksEditor::SlotSubtitle()
{
	emit SigClickSubtitle(m_ID);
}

void IconTracksEditor::ConnectSigToSlot()
{
//	ui->labelCoverArt->installEventFilter(this);
//	ui->labelTop->installEventFilter(this);
//	ui->labelBottom->installEventFilter(this);
//	ui->labelCount->installEventFilter(this);
//	ui->labelFavorite->installEventFilter(this);
//	ui->labelRating->installEventFilter(this);

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}
