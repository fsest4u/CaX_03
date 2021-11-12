#include "iconcoverarteditor.h"
#include "ui_iconcoverarteditor.h"

#include "widget/form/formsearchcoverart.h"

IconCoverArtEditor::IconCoverArtEditor(QWidget *parent) :
	QWidget(parent),
	m_FormSearchCoverArt(new FormSearchCoverArt(this)),
	ui(new Ui::IconCoverArtEditor)
{
	ui->setupUi(this);

	m_ID = -1;

	ui->gridLayoutFormSearchCoverArt->addWidget(m_FormSearchCoverArt);

	connect(m_FormSearchCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
}

IconCoverArtEditor::~IconCoverArtEditor()
{
	if (m_FormSearchCoverArt)
	{
		delete m_FormSearchCoverArt;
		m_FormSearchCoverArt = nullptr;
	}

	delete ui;
}

FormSearchCoverArt *IconCoverArtEditor::GetFormSearchCoverArt() const
{
	return m_FormSearchCoverArt;
}

int IconCoverArtEditor::GetID() const
{
	return m_ID;
}

void IconCoverArtEditor::SetID(int ID)
{
	m_ID = ID;
}

void IconCoverArtEditor::SlotCoverArt()
{
	emit SigClickCoverArt(m_ID);
}
