#include "browserdialog.h"
#include "ui_browserdialog.h"

#include "browserwidget.h"


BrowserDialog::BrowserDialog(QWidget *parent, const QString &addr, const int &eventID) :
	QDialog(parent),
	m_Addr(addr),
	m_EventID(eventID),
	ui(new Ui::BrowserDialog)
{
	ui->setupUi(this);

	m_Path = "";
}

BrowserDialog::~BrowserDialog()
{


	delete ui;
}

int BrowserDialog::GetBrowserMode() const
{
	return m_BrowserMode;
}

void BrowserDialog::SetBrowserMode(int BrowserMode, QString optionPath, int optionType)
{
	m_BrowserMode = BrowserMode;
}

void BrowserDialog::DoBrowserHome()
{
	BrowserWidget *widget = new BrowserWidget(this, m_Addr, m_EventID);
	widget->SetBrowserMode(m_BrowserMode);
	widget->RequestRoot();
	SlotAddWidget(widget);

	connect(widget, SIGNAL(SigSelectCoverart(QString)), this, SLOT(SlotSelectCoverart(QString)));
}

QString BrowserDialog::GetPath() const
{
	return m_Path;
}

void BrowserDialog::SetPath(const QString &Path)
{
	m_Path = Path;
}

void BrowserDialog::SlotAddWidget(QWidget *widget)
{
	auto idx = ui->stackDialog->currentIndex();
	auto cnt = ui->stackDialog->count() - 1;

	// remove the widget after the current wiget
	if (cnt > idx)
	{
		for (int i = cnt; i > idx; i--)
		{
			auto backWidget = ui->stackDialog->widget(i);
			ui->stackDialog->removeWidget(backWidget);
		}
	}

	idx = ui->stackDialog->addWidget(widget);
	ui->stackDialog->setCurrentIndex(idx);

	UpdateStackState();
}

void BrowserDialog::SlotRemoveWidget(QWidget *widget)
{
	ui->stackDialog->removeWidget(widget);

	UpdateStackState();
}

void BrowserDialog::SlotSelectCoverart(QString path)
{
	m_Path = path;

	emit accept();
}

void BrowserDialog::UpdateStackState()
{
	auto index = ui->stackDialog->currentIndex();
	auto count = ui->stackDialog->count();

	if (index < 0 || count <= 0)
	{
		return;
	}

	// Update buttons depending on the page count.
	auto hasPage = count > 0;
	if (!hasPage)
	{
		ui->btnNext->setEnabled(false);
		ui->btnPrev->setEnabled(false);
		return;
	}

	// Update buttons depending on the current page index.
	auto isLastPage = index == count - 1;
	auto isFirstPage = index == 0;
	if (isLastPage && isFirstPage)
	{
		ui->btnNext->setEnabled(false);
		ui->btnPrev->setEnabled(false);
		return;
	}

	if (isLastPage)
	{
		ui->btnNext->setEnabled(false);
		ui->btnPrev->setEnabled(true);
		return;
	}

	if (isFirstPage)
	{
		ui->btnNext->setEnabled(true);
		ui->btnPrev->setEnabled(false);
		return;
	}

	ui->btnNext->setEnabled(true);
	ui->btnPrev->setEnabled(true);
}
