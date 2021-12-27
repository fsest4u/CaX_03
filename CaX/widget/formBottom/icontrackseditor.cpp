#include <QMouseEvent>

#include "icontrackseditor.h"
#include "ui_icontrackseditor.h"

#include "util/log.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"

IconTracksEditor::IconTracksEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	ui(new Ui::IconTracksEditor)
{
	ui->setupUi(this);

	m_ID = -1;

	ConnectSigToSlot();

}

IconTracksEditor::~IconTracksEditor()
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

int IconTracksEditor::GetID()
{
	return m_ID;
}

void IconTracksEditor::SetID(int nID)
{
	m_ID = nID;
}

FormTitle *IconTracksEditor::GetFormTitle() const
{
	return m_pFormTitle;
}

FormCoverArt *IconTracksEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

void IconTracksEditor::SlotCoverArtLeft(int index)
{
	Q_UNUSED(index)
	emit SigClickTitle(m_ID, m_pFormCoverArt->GetCoverArt());
}

void IconTracksEditor::SlotCoverArtRight(int index)
{
	emit SigClickCoverArt(index);
}

void IconTracksEditor::SlotRating(int nRating)
{
	emit SigClickRating(m_ID, nRating);
}

void IconTracksEditor::SlotFavorite(int nFavorite)
{
	if (nFavorite == 0)
	{
		nFavorite = 1;
	}
	else
	{
		nFavorite = 0;
	}
	m_pFormCoverArt->SetFavorite(nFavorite);

	emit SigClickFavorite(m_ID, nFavorite);
}

void IconTracksEditor::SlotPlay()
{
	emit SigClickPlay(m_ID);
}

void IconTracksEditor::SlotTitle()
{
	emit SigClickTitle(m_ID, m_pFormCoverArt->GetCoverArt());
}

void IconTracksEditor::SlotSubtitle()
{
	emit SigClickSubtitle(m_ID, m_pFormCoverArt->GetCoverArt());
}

void IconTracksEditor::ConnectSigToSlot()
{

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArtLeft(int)), this, SLOT(SlotCoverArtLeft(int)));
	connect(m_pFormCoverArt, SIGNAL(SigCoverArtRight(int)), this, SLOT(SlotCoverArtRight(int)));
	connect(m_pFormCoverArt, SIGNAL(SigRating(int)), this, SLOT(SlotRating(int)));
	connect(m_pFormCoverArt, SIGNAL(SigFavorite(int)), this, SLOT(SlotFavorite(int)));
	connect(m_pFormCoverArt, SIGNAL(SigPlay()), this, SLOT(SlotPlay()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));

}

