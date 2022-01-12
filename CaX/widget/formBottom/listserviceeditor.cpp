#include <QMouseEvent>

#include "listserviceeditor.h"
#include "ui_listserviceeditor.h"

#include "widget/form/formcoverart.h"

#include "util/log.h"



ListServiceEditor::ListServiceEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	ui(new Ui::ListServiceEditor)
{
	ui->setupUi(this);

	m_nType = -1;
	m_RawData.clear();

	ConnectSigToSlot();

	if (ui->frameFormCoverArt->width() <= 60)
	{
		m_pFormCoverArt->SetMinimumSizeCheck(20);
	}
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

void ListServiceEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

QString ListServiceEditor::GetSubtitle() const
{
	return ui->labelSubtitle->text();
}

void ListServiceEditor::SetSubtitle(const QString &subtitle)
{
	ui->labelSubtitle->show();
	ui->labelSubtitle->setText(subtitle);
}

QString ListServiceEditor::GetDuration() const
{
	return ui->labelDuration->text();
}

void ListServiceEditor::SetDuration(const QString &duration)
{
	ui->labelDuration->show();
	ui->labelDuration->setText(duration);
}


QString ListServiceEditor::GetRawData() const
{
	return m_RawData;
}

void ListServiceEditor::SetRawData(const QString &rawData)
{
	m_RawData = rawData;
}

FormCoverArt *ListServiceEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_RawData);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListServiceEditor::SlotCoverArt(int index)
{
	Q_UNUSED(index)
	emit SigClickTitle(m_RawData);
}

void ListServiceEditor::SlotCheck(int index)
{
	emit SigClickCoverArt(index);
}

void ListServiceEditor::ConnectSigToSlot()
{
	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->hide();
	ui->labelDuration->hide();

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt(int)), this, SLOT(SlotCoverArt(int)));
	connect(m_pFormCoverArt, SIGNAL(SigCheck(int)), this, SLOT(SlotCheck(int)));

}

