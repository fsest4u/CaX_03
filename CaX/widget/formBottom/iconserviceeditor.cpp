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

QString IconServiceEditor::GetRawData() const
{
	return m_RawData;
}

void IconServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

FormTitle *IconServiceEditor::GetFormTitle() const
{
	return m_pFormTitle;
}

FormCoverArt *IconServiceEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

void IconServiceEditor::SlotCoverArt(int index)
{
	Q_UNUSED(index)
	emit SigClickTitle(m_Type, m_RawData);
}

void IconServiceEditor::SlotCheck(int index)
{
	emit SigClickCoverArt(index);
}

void IconServiceEditor::SlotPlay()
{
	emit SigClickPlay(m_ID, m_pFormCoverArt->GetMute());
}

void IconServiceEditor::SlotTitle()
{
	emit SigClickTitle(m_Type, m_RawData);
}


void IconServiceEditor::ConnectSigToSlot()
{

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt(int)), this, SLOT(SlotCoverArt(int)));
	connect(m_pFormCoverArt, SIGNAL(SigCheck(int)), this, SLOT(SlotCheck(int)));
	connect(m_pFormCoverArt, SIGNAL(SigPlay()), this, SLOT(SlotPlay()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotTitle()));

}
