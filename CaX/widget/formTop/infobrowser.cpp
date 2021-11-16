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
	m_pFormTitle(new FormTitle(this)),
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

	if (m_pFormTitle)
	{
		delete m_pFormTitle;
		m_pFormTitle = nullptr;
	}

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

void InfoBrowser::SetTitle(const QString title)
{
	m_pFormTitle->SetTitleFont(42);
	m_pFormTitle->SetTitle(title);
}

void InfoBrowser::SetSubtitle(const QString title)
{
	m_pFormTitle->SetSubtitleFont(42);
	m_pFormTitle->SetSubtitle(title);
}

FormPlay *InfoBrowser::GetFormPlay()
{
	return m_pFormPlay;

}

void InfoBrowser::SlotCoverArt()
{
	LogDebug("good choice cover art");
}

void InfoBrowser::SlotTitle()
{
	LogDebug("good choice title");
}

void InfoBrowser::SlotSubtitle()
{
	LogDebug("good choice subtitle");

}

void InfoBrowser::ConnectSigToSlot()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);

//	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
//	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}
