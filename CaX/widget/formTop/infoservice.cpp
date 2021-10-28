#include <QMouseEvent>

#include "infoservice.h"
#include "ui_infoservice.h"

#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoService::InfoService(QWidget *parent) :
	QWidget(parent),
	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoService)
{
	ui->setupUi(this);

	ConnectSigToSlot();

}

InfoService::~InfoService()
{


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

void InfoService::SetTitle(const QString title)
{
	m_pFormTitle->SetTitleFontSize(42);
	m_pFormTitle->SetTitle(title);
}

void InfoService::SetSubtitle(const QString title)
{
	m_pFormTitle->SetSubtitleFontSize(42);
	m_pFormTitle->SetSubtitle(title);
}

void InfoService::SlotTitle()
{
	LogDebug("good choice title");
}

void InfoService::SlotSubtitle()
{
	LogDebug("good choice subtitle");
}


void InfoService::ConnectSigToSlot()
{
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));
}


FormPlay *InfoService::GetFormPlay()
{
	return m_pFormPlay;
}

FormSort *InfoService::GetFormSort()
{
	return m_pFormSort;
}
