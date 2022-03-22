#include "progressdialog.h"
#include "ui_progressdialog.h"

#include "util/log.h"
#include "util/utilnovatron.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProgressDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

ProgressDialog::~ProgressDialog()
{
	delete ui;
}

void ProgressDialog::SetWindowTitle(QString title)
{
	setWindowTitle(title);
}

void ProgressDialog::SetContent(QString content)
{
	ui->labelContent->setText(content);
}

void ProgressDialog::SetCurTime(int seconds)
{
	QString time = UtilNovatron::CalcSecondToHMS(seconds);
	ui->labelCurTime->setText(time);
}

void ProgressDialog::SetTotTime(int seconds)
{
	QString time = UtilNovatron::CalcSecondToHMS(seconds);
	ui->labelTotTime->setText(time);
}

void ProgressDialog::SetProgress(int cur, int tot)
{
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(tot);
	ui->progressBar->setValue(cur);
}

void ProgressDialog::SetBtnBack(bool show)
{
	if (show)
	{
		ui->btnBack->show();
	}
	else
	{
		ui->btnBack->hide();
	}
}

void ProgressDialog::SetBtnStop(bool show)
{
	if (show)
	{
		ui->btnStop->show();
	}
	else
	{
		ui->btnStop->hide();
	}
}

void ProgressDialog::SetBtnSkip(bool show)
{
	if (show)
	{
		ui->btnSkip->show();
	}
	else
	{
		ui->btnSkip->hide();
	}
}

void ProgressDialog::SetBtnClose(bool show)
{
	if (show)
	{
		ui->btnClose->show();
	}
	else
	{
		ui->btnClose->hide();
	}
}

void ProgressDialog::SetTaskID(int taskID)
{
	m_TaskID = taskID;
}

void ProgressDialog::SetEventID(int eventID)
{
	m_EventID = eventID;
}

void ProgressDialog::SlotClickBack()
{
	emit SigClickBack(m_TaskID);
}

void ProgressDialog::SlotClickStop()
{
	emit SigClickStop(m_TaskID);
}

void ProgressDialog::SlotClickSkip()
{
	emit SigClickSkip(m_TaskID);
}

void ProgressDialog::SlotClickClose()
{
	emit SigClickClose();
	this->close();
}

void ProgressDialog::ConnectSigToSlot()
{
	connect(ui->btnBack, SIGNAL(clicked()), this, SLOT(SlotClickBack()));
	connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(SlotClickStop()));
	connect(ui->btnSkip, SIGNAL(clicked()), this, SLOT(SlotClickSkip()));
	connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(SlotClickClose()));

}

void ProgressDialog::Initialize()
{
	m_EventID = -1;

}
