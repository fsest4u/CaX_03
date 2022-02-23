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
//	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoService)
{
	ui->setupUi(this);

	ConnectSigToSlot();

}

InfoService::~InfoService()
{
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

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

	delete ui;

}

void InfoService::SetTitle(const QString title)
{
//	m_pFormTitle->SetTitleFont(FONT_SIZE_INFO_TITLE);
//	m_pFormTitle->SetTitle(title);
	ui->labelTitle->setText(title);
}

//void InfoService::SetSubtitle(const QString title)
//{
////	m_pFormTitle->SetSubtitleFont(FONT_SIZE_INFO_SUBTITLE);
////	m_pFormTitle->SetSubtitle(title);
//	ui->labelSubtitle->setText(title);
//	ui->labelSubtitle->startTimer();
//}

FormPlay *InfoService::GetFormPlay()
{
	return m_pFormPlay;
}

FormSort *InfoService::GetFormSort()
{
	return m_pFormSort;
}

bool InfoService::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::Enter)
	{
		if (object == ui->labelTitle)
		{
			ui->labelTitle->startTimer();
		}

	}
	else if (event->type() == QMouseEvent::Leave)
	{
		if (object == ui->labelTitle)
		{
			ui->labelTitle->stopTimer();
		}

	}

	return QObject::eventFilter(object, event);
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
//	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	ui->labelTitle->installEventFilter(this);

//	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
//	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));
}

