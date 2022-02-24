#include <QMouseEvent>

#include "infobrowser.h"
#include "ui_infobrowser.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
//#include "widget/form/formsort.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoBrowser::InfoBrowser(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
//	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	ui(new Ui::InfoBrowser)
{
	ui->setupUi(this);

	ConnectSigToSlot();
}

InfoBrowser::~InfoBrowser()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

//	if (m_pFormTitle)
//	{
//		delete m_pFormTitle;
//		m_pFormTitle = nullptr;
//	}

	if (m_pFormPlay)
	{
		delete m_pFormPlay;
		m_pFormPlay = nullptr;
	}

	delete ui;
}

void InfoBrowser::SetCoverArt(QString coverArt)
{
	m_pFormCoverArt->SetCoverArt(coverArt);
}

QString InfoBrowser::GetTitle() const
{
//	return m_pFormTitle->GetTitle();
	return ui->labelTitle->text();
}

void InfoBrowser::SetTitle(const QString title)
{
//	m_pFormTitle->SetTitleFont(FONT_SIZE_INFO_TITLE);
//	m_pFormTitle->SetTitle(title);
	ui->labelTitle->setText(title);
}

QString InfoBrowser::GetSubtitle() const
{
//	return m_pFormTitle->GetSubtitle();
	return ui->labelSubtitle->text();
}

void InfoBrowser::SetSubtitle(const QString title)
{
//	m_pFormTitle->SetSubtitleFont(FONT_SIZE_INFO_SUBTITLE);
//	m_pFormTitle->SetSubtitle(title);
	ui->labelSubtitle->setText(title);
}

FormPlay *InfoBrowser::GetFormPlay()
{
	return m_pFormPlay;

}

bool InfoBrowser::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::Enter)
	{
		if (object == ui->labelTitle)
		{
			ui->labelTitle->startTimer();
		}
		else if (object == ui->labelSubtitle)
		{
			ui->labelSubtitle->startTimer();
		}
	}
	else if (event->type() == QMouseEvent::Leave)
	{
		if (object == ui->labelTitle)
		{
			ui->labelTitle->stopTimer();
		}
		else if (object == ui->labelSubtitle)
		{
			ui->labelSubtitle->stopTimer();
		}
	}

	return QObject::eventFilter(object, event);
}

void InfoBrowser::SlotCoverArt()
{
//	LogDebug("good choice cover art");
}

void InfoBrowser::SlotTitle()
{
//	LogDebug("good choice title");
}

void InfoBrowser::SlotSubtitle()
{
//	LogDebug("good choice subtitle");

}

void InfoBrowser::ConnectSigToSlot()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
//	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);

	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->installEventFilter(this);

//	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
//	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}
