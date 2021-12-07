#include "searchcategoryeditor.h"
#include "ui_searchcategoryeditor.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"

#include "util/log.h"

SearchCategoryEditor::SearchCategoryEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	ui(new Ui::SearchCategoryEditor)
{
	ui->setupUi(this);

	m_ID = -1;

	ConnectSigToSlot();
}

SearchCategoryEditor::~SearchCategoryEditor()
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

int SearchCategoryEditor::GetID() const
{
	return m_ID;
}

void SearchCategoryEditor::SetID(int ID)
{
	m_ID = ID;
}

FormCoverArt *SearchCategoryEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

FormTitle *SearchCategoryEditor::GetFormTitle() const
{
	return m_pFormTitle;
}

void SearchCategoryEditor::SlotTitle()
{
	emit SigClickTitle(m_ID, m_pFormCoverArt->GetCoverArt());
}

void SearchCategoryEditor::ConnectSigToSlot()
{
	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotTitle()));
}

