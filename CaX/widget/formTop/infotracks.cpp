#include <QMouseEvent>

#include "infotracks.h"
#include "ui_infotracks.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "util/caxtranslate.h"
#include "util/log.h"

InfoTracks::InfoTracks(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoTracks)
{
	ui->setupUi(this);

	ConnectSigToSlot();

}

InfoTracks::~InfoTracks()
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

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

	delete ui;

}

void InfoTracks::SetTitle(QString title)
{
	if (title.isEmpty())
	{
		title = STR_UNKNOWN;
	}
	m_pFormTitle->SetTitleFont(42);
	m_pFormTitle->SetTitle(title);
}

void InfoTracks::SetSubtitle(QString subtitle)
{
	if (subtitle.isEmpty())
	{
		subtitle = STR_UNKNOWN;
	}
	m_pFormTitle->SetSubtitleFont(20);
	m_pFormTitle->SetSubtitle(subtitle);
}

//void InfoTracks::SetInfo(QString info)
//{
//	if (info.isEmpty())
//	{
//		info = tr("-");
//	}
//	ui->labelInfo->setText(info);
//}

void InfoTracks::SetCoverArt(QString coverArt)
{
	m_pFormCoverArt->SetCoverArt(coverArt);
}

FormPlay *InfoTracks::GetFormPlay()
{
	return m_pFormPlay;
}

FormSort *InfoTracks::GetFormSort()
{
	return m_pFormSort;
}

void InfoTracks::SlotCoverArt()
{
	LogDebug("good choice cover art");
}

void InfoTracks::SlotTitle()
{
	LogDebug("good choice title");

}

void InfoTracks::SlotSubtitle()
{
	LogDebug("good choice subtitle");

}

void InfoTracks::ConnectSigToSlot()
{

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}
