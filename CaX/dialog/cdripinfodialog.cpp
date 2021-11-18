#include "cdripinfodialog.h"
#include "ui_cdripinfodialog.h"

#include "dialog/cdripinfo.h"
#include "dialog/trackinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

CDRipInfoDialog::CDRipInfoDialog(QWidget *parent) :
	QDialog(parent),
	m_WidgetCD(new CDRipInfo),
	ui(new Ui::CDRipInfoDialog)
{
	ui->setupUi(this);

	m_InfoData.Clear();
	m_ListTrack.clear();

	connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(SlotBtnPrev()));
	connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(SlotBtnNext()));

	UpdateStackState();
}

CDRipInfoDialog::~CDRipInfoDialog()
{
	if (m_WidgetCD)
	{
		delete m_WidgetCD;
		m_WidgetCD = nullptr;
	}

	delete ui;
}

CJsonNode CDRipInfoDialog::GetInfoData()
{
	return m_InfoData;
}

void CDRipInfoDialog::SetInfoData(CJsonNode node)
{
	m_InfoData = node;
	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());
	m_WidgetCD->SetAddr(m_Addr);
	m_WidgetCD->SetAlbumList(m_AlbumList);
	m_WidgetCD->SetAlbumArtistList(m_AlbumArtistList);
	m_WidgetCD->SetInfoData(m_InfoData);
	ui->stackedWidget->addWidget(m_WidgetCD);

	connect(m_WidgetCD, SIGNAL(SigChangeFormat(int)), this, SLOT(SlotChangeFormat(int)));
	connect(m_WidgetCD, SIGNAL(SigChangeAlbum(QString)), this, SLOT(SlotChangeAlbum(QString)));
	connect(m_WidgetCD, SIGNAL(SigChangeAlbumArtist(QString)), this, SLOT(SlotChangeAlbumArtist(QString)));
	connect(m_WidgetCD, SIGNAL(SigChangeCDYear(QString)), this, SLOT(SlotChangeCDYear(QString)));
	connect(m_WidgetCD, SIGNAL(SigChangeCDNumber(QString)), this, SLOT(SlotChangeCDNumber(QString)));
	connect(m_WidgetCD, SIGNAL(SigChangeCDTotal(QString)), this, SLOT(SlotChangeCDTotal(QString)));
	connect(m_WidgetCD, SIGNAL(SigChangeCoverArt(QString, QString)), this, SLOT(SlotChangeCoverArt(QString, QString)));

	CJsonNode tracks = m_InfoData.GetArray(KEY_TRACKS);
	LogDebug("tracks [%s]", tracks.ToCompactByteArray().data());
	for (int i = 0; i < tracks.ArraySize(); i++)
	{
		TrackInfo *trackInfo = new TrackInfo();
		trackInfo->SetIndex(i);
		trackInfo->SetArtistList(m_ArtistList);
		trackInfo->SetGenreList(m_GenreList);
		trackInfo->SetComposerList(m_ComposerList);
		trackInfo->SetMoodList(m_MoodList);
		trackInfo->SetMode(TrackInfo::TRACK_INFO_MODE_EDIT);
		trackInfo->SetInfoData(tracks.GetArrayAt(i));
		m_ListTrack.append(tracks.GetArrayAt(i));
		ui->stackedWidget->addWidget(trackInfo);

		connect(trackInfo, SIGNAL(SigChangeArtist(int, QString)), this, SLOT(SlotChangeArtist(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeGenre(int, QString)), this, SLOT(SlotChangeGenre(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeComposer(int, QString)), this, SLOT(SlotChangeComposer(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeMood(int, QString)), this, SLOT(SlotChangeMood(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeTitle(int, QString)), this, SLOT(SlotChangeTitle(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeTempo(int, QString)), this, SLOT(SlotChangeTempo(int, QString)));
		connect(trackInfo, SIGNAL(SigChangeYear(int, QString)), this, SLOT(SlotChangeYear(int, QString)));

	}

	UpdateStackState();
}

void CDRipInfoDialog::SlotBtnPrev()
{
	auto idx = ui->stackedWidget->currentIndex();
	ui->stackedWidget->setCurrentIndex(--idx);

	UpdateStackState();
}

void CDRipInfoDialog::SlotBtnNext()
{
	auto idx = ui->stackedWidget->currentIndex();
	ui->stackedWidget->setCurrentIndex(++idx);

	UpdateStackState();
}

void CDRipInfoDialog::SlotChangeFormat(int index)
{
	m_InfoData.AddInt(KEY_FORMAT, index);
}

void CDRipInfoDialog::SlotChangeAlbum(QString value)
{
	m_InfoData.Add(KEY_ALBUM, value);
}

void CDRipInfoDialog::SlotChangeAlbumArtist(QString value)
{
	m_InfoData.Add(KEY_ALBUM_ARTIST, value);
}

void CDRipInfoDialog::SlotChangeCDYear(QString value)
{
	m_InfoData.Add(KEY_CDYEAR, value);
}

void CDRipInfoDialog::SlotChangeCDNumber(QString value)
{
	m_InfoData.Add(KEY_CDNUMBER, value);
}

void CDRipInfoDialog::SlotChangeCDTotal(QString value)
{
	m_InfoData.Add(KEY_CDTOTAL, value);
}

void CDRipInfoDialog::SlotChangeCoverArt(QString image, QString thumb)
{
	CJsonNode coverArt(JSON_OBJECT);
	coverArt.Add("ImageUrl", image);
	coverArt.Add("ThumUrl", thumb);

	m_InfoData.Add("Coverart", coverArt);
}

void CDRipInfoDialog::SlotChangeArtist(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_ARTIST, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeGenre(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_GENRE, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeComposer(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_COMPOSER, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeMood(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_MOOD, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeTitle(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_TITLE_CAP, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeTempo(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_TEMPO, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::SlotChangeYear(int index, QString value)
{
	CJsonNode node = m_ListTrack.at(index);
	node.Add(KEY_YEAR, value);
	LogDebug("node [%s]", node.ToCompactByteArray().data());
	m_ListTrack.replace(index, node);
}

void CDRipInfoDialog::accept()
{
	CJsonNode trackArr(JSON_ARRAY);
	for(int i = 0; i < m_ListTrack.count(); i++)
	{
		trackArr.AppendArray(m_ListTrack.at(i));
	}
	m_InfoData.Del(KEY_TRACKS);
	m_InfoData.Add(KEY_TRACKS, trackArr);

	done(1);
}

void CDRipInfoDialog::reject()
{
	done(0);
}

void CDRipInfoDialog::UpdateStackState()
{
	auto idx = ui->stackedWidget->currentIndex();
	auto cnt = ui->stackedWidget->count();

//	ui->widgetTop->SetTitle(idx);

	// Update buttons depending on the page count.
	auto hasPage = cnt > 0;
	if (!hasPage)
	{
		ui->btnNext->setEnabled(false);
		ui->btnPrev->setEnabled(false);
		return;
	}

	// Update buttons depending on the current page index.
	auto isLastPage = idx == cnt - 1;
	auto isFirstPage = idx == 0;
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

QString CDRipInfoDialog::GetAddr() const
{
	return m_Addr;
}

void CDRipInfoDialog::SetAddr(const QString &Addr)
{
	m_Addr = Addr;
}

QStringList CDRipInfoDialog::GetMoodList() const
{
	return m_MoodList;
}

void CDRipInfoDialog::SetMoodList(const QStringList &MoodList)
{
	m_MoodList = MoodList;
}

QStringList CDRipInfoDialog::GetComposerList() const
{
	return m_ComposerList;
}

void CDRipInfoDialog::SetComposerList(const QStringList &ComposerList)
{
	m_ComposerList = ComposerList;
}

QStringList CDRipInfoDialog::GetGenreList() const
{
	return m_GenreList;
}

void CDRipInfoDialog::SetGenreList(const QStringList &GenreList)
{
	m_GenreList = GenreList;
}

QStringList CDRipInfoDialog::GetArtistList() const
{
	return m_ArtistList;
}

void CDRipInfoDialog::SetArtistList(const QStringList &ArtistList)
{
	m_ArtistList = ArtistList;
}

QStringList CDRipInfoDialog::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void CDRipInfoDialog::SetAlbumArtistList(const QStringList &AlbumArtistList)
{
	m_AlbumArtistList = AlbumArtistList;
}

QStringList CDRipInfoDialog::GetAlbumList() const
{
	return m_AlbumList;
}

void CDRipInfoDialog::SetAlbumList(const QStringList &AlbumList)
{
	m_AlbumList = AlbumList;
}
