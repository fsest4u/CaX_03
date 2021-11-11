#include "cdriptrackinfo.h"
#include "ui_cdriptrackinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

CDRipTrackInfo::CDRipTrackInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CDRipTrackInfo)
{
	ui->setupUi(this);

	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

	connect(ui->cbArtist, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedArtist(int)));
	connect(ui->cbGenre, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedGenre(int)));
	connect(ui->cbComposer, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedComposer(int)));
	connect(ui->cbMood, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedMood(int)));
	connect(ui->lineEditTitle, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishTitle()));
	connect(ui->lineEditTempo, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishTempo()));
	connect(ui->lineEditYear, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishYear()));

}

CDRipTrackInfo::~CDRipTrackInfo()
{
	delete ui;
}

CJsonNode CDRipTrackInfo::GetInfoData()
{
	return m_InfoData;
}

void CDRipTrackInfo::SetInfoData(CJsonNode node)
{
	m_InfoData = node;
	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());

	if (!m_InfoData.GetString(KEY_ARTIST).isEmpty())
		ui->cbArtist->addItem(m_InfoData.GetString(KEY_ARTIST));
	ui->cbArtist->addItems(m_ArtistList);
	ui->cbArtist->setCurrentIndex(0);

	if (!m_InfoData.GetString(KEY_GENRE).isEmpty())
		ui->cbGenre->addItem(m_InfoData.GetString(KEY_GENRE));
	ui->cbGenre->addItems(m_GenreList);
	ui->cbGenre->setCurrentIndex(0);

	if (!m_InfoData.GetString(KEY_COMPOSER).isEmpty())
		ui->cbComposer->addItem(m_InfoData.GetString(KEY_COMPOSER));
	ui->cbComposer->addItems(m_ComposerList);
	ui->cbComposer->setCurrentIndex(0);

	if (!m_InfoData.GetString(KEY_MOOD).isEmpty())
		ui->cbMood->addItem(m_InfoData.GetString(KEY_MOOD));
	ui->cbMood->addItems(m_MoodList);
	ui->cbMood->setCurrentIndex(0);

	ui->lineEditTitle->setText(m_InfoData.GetString(KEY_TITLE_CAP));
	ui->lineEditTempo->setText(m_InfoData.GetString(KEY_TEMPO));
	ui->lineEditYear->setText(m_InfoData.GetString(KEY_YEAR));
}

QStringList CDRipTrackInfo::GetArtistList() const
{
	return m_ArtistList;
}

void CDRipTrackInfo::SetArtistList(const QStringList &ArtistList)
{
	m_ArtistList = ArtistList;
}

QStringList CDRipTrackInfo::GetGenreList() const
{
	return m_GenreList;
}

void CDRipTrackInfo::SetGenreList(const QStringList &GenreList)
{
	m_GenreList = GenreList;
}

QStringList CDRipTrackInfo::GetComposerList() const
{
	return m_ComposerList;
}

void CDRipTrackInfo::SetComposerList(const QStringList &ComposerList)
{
	m_ComposerList = ComposerList;
}

QStringList CDRipTrackInfo::GetMoodList() const
{
	return m_MoodList;
}

void CDRipTrackInfo::SetMoodList(const QStringList &MoodList)
{
	m_MoodList = MoodList;
}

int CDRipTrackInfo::GetIndex() const
{
	return m_Index;
}

void CDRipTrackInfo::SetIndex(int Index)
{
	m_Index = Index;
}

void CDRipTrackInfo::SlotChangedArtist(int index)
{
	emit SigChangeArtist(m_Index, ui->cbArtist->itemText(index));
}

void CDRipTrackInfo::SlotChangedGenre(int index)
{
	emit SigChangeGenre(m_Index, ui->cbGenre->itemText(index));
}

void CDRipTrackInfo::SlotChangedComposer(int index)
{
	emit SigChangeComposer(m_Index, ui->cbComposer->itemText(index));
}

void CDRipTrackInfo::SlotChangedMood(int index)
{
	emit SigChangeMood(m_Index, ui->cbMood->itemText(index));
}

void CDRipTrackInfo::SlotEditFinishTitle()
{
	emit SigChangeTitle(m_Index, ui->lineEditTitle->text());
}

void CDRipTrackInfo::SlotEditFinishTempo()
{
	emit SigChangeTempo(m_Index, ui->lineEditTempo->text());
}

void CDRipTrackInfo::SlotEditFinishYear()
{
	emit SigChangeYear(m_Index, ui->lineEditYear->text());
}
