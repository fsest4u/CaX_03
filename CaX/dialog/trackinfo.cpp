#include "trackinfo.h"
#include "ui_trackinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

TrackInfo::TrackInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TrackInfo)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	Initialize();

}

TrackInfo::~TrackInfo()
{
	delete ui;
}

CJsonNode TrackInfo::GetInfoData()
{
	return m_InfoData;
}

void TrackInfo::SetInfoData(CJsonNode node)
{
	m_InfoData = node;
//	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());

	SetAlbum(m_AlbumList, m_InfoData.GetString(KEY_ALBUM));
	SetAlbumArtist(m_AlbumArtistList, m_InfoData.GetString(KEY_ALBUM_ARTIST));
	SetArtist(m_ArtistList, m_InfoData.GetString(KEY_ARTIST));
	SetComposer(m_ComposerList, m_InfoData.GetString(KEY_COMPOSER));
	SetGenre(m_GenreList, m_InfoData.GetString(KEY_GENRE));
	SetMood(m_MoodList, m_InfoData.GetString(KEY_MOOD));

	SetTitle(m_InfoData.GetString(KEY_TITLE_CAP));
	SetTempo(m_InfoData.GetString(KEY_TEMPO));
	SetYear(m_InfoData.GetString(KEY_YEAR));
	SetTrack(m_InfoData.GetString(KEY_TRACK));
	SetPath(m_InfoData.GetString(KEY_PATH));
	SetBitrate(m_InfoData.GetString(KEY_BITRATE));
	SetDuration(m_InfoData.GetString(KEY_DURATION));
	SetSampleRate(m_InfoData.GetString(KEY_SAMPLERATE));
	SetChannel(m_InfoData.GetString(KEY_CHANNEL));
	SetFormat(m_InfoData.GetString(KEY_FORMAT));
	SetCDYear(m_InfoData.GetString(KEY_CD_YEAR));
	SetCDNumber(m_InfoData.GetString(KEY_CD_NUMBER));
	SetCDTotal(m_InfoData.GetString(KEY_CD_TOTAL));

}

QStringList TrackInfo::GetAlbumList() const
{
	return m_AlbumList;
}

void TrackInfo::SetAlbumList(const QStringList &list)
{
	m_AlbumList = list;
}

QStringList TrackInfo::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void TrackInfo::SetAlbumArtistList(const QStringList &list)
{
	m_AlbumArtistList = list;
}

QStringList TrackInfo::GetArtistList() const
{
	return m_ArtistList;
}

void TrackInfo::SetArtistList(const QStringList &list)
{
	m_ArtistList = list;
}

QStringList TrackInfo::GetGenreList() const
{
	return m_GenreList;
}

void TrackInfo::SetGenreList(const QStringList &list)
{
	m_GenreList = list;
}

QStringList TrackInfo::GetComposerList() const
{
	return m_ComposerList;
}

void TrackInfo::SetComposerList(const QStringList &list)
{
	m_ComposerList = list;
}

QStringList TrackInfo::GetMoodList() const
{
	return m_MoodList;
}

void TrackInfo::SetMoodList(const QStringList &list)
{
	m_MoodList = list;
}

int TrackInfo::GetIndex() const
{
	return m_Index;
}

void TrackInfo::SetIndex(int Index)
{
	m_Index = Index;
}

//void TrackInfo::SlotChangedAlbum(int index)
//{
//	emit SigChangeAlbum(m_Index, ui->cbAlbum->itemText(index));
//}

//void TrackInfo::SlotChangedAlbumArtist(int index)
//{
//	emit SigChangeAlbumArtist(m_Index, ui->cbAlbumArtist->itemText(index));
//}

//void TrackInfo::SlotChangedArtist(int index)
//{
//	emit SigChangeArtist(m_Index, ui->cbArtist->itemText(index));
//}

//void TrackInfo::SlotChangedGenre(int index)
//{
//	emit SigChangeGenre(m_Index, ui->cbGenre->itemText(index));
//}

//void TrackInfo::SlotChangedComposer(int index)
//{
//	emit SigChangeComposer(m_Index, ui->cbComposer->itemText(index));
//}

//void TrackInfo::SlotChangedMood(int index)
//{
//	emit SigChangeMood(m_Index, ui->cbMood->itemText(index));
//}

void TrackInfo::SlotEditTextChangedAlbum(const QString &text)
{
	emit SigChangeAlbum(m_Index, text);
}

void TrackInfo::SlotEditTextChangedAlbumArtist(const QString &text)
{
	emit SigChangeAlbumArtist(m_Index, text);
}

void TrackInfo::SlotEditTextChangedArtist(const QString &text)
{
	emit SigChangeArtist(m_Index, text);
}

void TrackInfo::SlotEditTextChangedComposer(const QString &text)
{
	emit SigChangeComposer(m_Index, text);
}

void TrackInfo::SlotEditTextChangedGenre(const QString &text)
{
	emit SigChangeGenre(m_Index, text);
}

void TrackInfo::SlotEditTextChangedMood(const QString &text)
{
	emit SigChangeMood(m_Index, text);
}

void TrackInfo::SlotEditFinishTitle()
{
	emit SigChangeTitle(m_Index, ui->lineEditTitle->text());
}

void TrackInfo::SlotEditFinishTempo()
{
	emit SigChangeTempo(m_Index, ui->lineEditTempo->text());
}

void TrackInfo::SlotEditFinishYear()
{
	emit SigChangeYear(m_Index, ui->lineEditYear->text());
}

void TrackInfo::SlotEditFinishTrack()
{
	emit SigChangeTrack(m_Index, ui->lineEditTrack->text());
}

void TrackInfo::SlotEditFinishPath()
{
	emit SigChangePath(m_Index, ui->lineEditPath->text());
}

void TrackInfo::SlotEditFinishBitrate()
{
	emit SigChangeBitrate(m_Index, ui->lineEditBitrate->text());
}

void TrackInfo::SlotEditFinishDuration()
{
	emit SigChangeDuration(m_Index, ui->lineEditDuration->text());
}

void TrackInfo::SlotEditFinishSampleRate()
{
	emit SigChangeSampleRate(m_Index, ui->lineEditSampleRate->text());
}

void TrackInfo::SlotEditFinishChannel()
{
	emit SigChangeChannel(m_Index, ui->lineEditChannel->text());
}

void TrackInfo::SlotEditFinishFormat()
{
	emit SigChangeFormat(m_Index, ui->lineEditFormat->text());
}

void TrackInfo::SlotEditFinishCDYear()
{
	emit SigChangeCDYear(m_Index, ui->lineEditCDYear->text());
}

void TrackInfo::SlotEditFinishCDNumber()
{
	emit SigChangeCDNumber(m_Index, ui->lineEditCDNumber->text());
}

void TrackInfo::SlotEditFinishCDTotal()
{
	emit SigChangeCDTotal(m_Index, ui->lineEditCDTotal->text());
}

void TrackInfo::ConnectSigToSlot()
{
//	connect(ui->cbAlbum, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedAlbum(int)));
//	connect(ui->cbAlbumArtist, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedAlbumArtist(int)));
//	connect(ui->cbArtist, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedArtist(int)));
//	connect(ui->cbGenre, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedGenre(int)));
//	connect(ui->cbComposer, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedComposer(int)));
//	connect(ui->cbMood, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedMood(int)));

	connect(ui->cbAlbum, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedAlbum(const QString &)));
	connect(ui->cbAlbumArtist, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedAlbumArtist(const QString &)));
	connect(ui->cbArtist, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedArtist(const QString &)));
	connect(ui->cbGenre, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedGenre(const QString &)));
	connect(ui->cbComposer, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedComposer(const QString &)));
	connect(ui->cbMood, SIGNAL(editTextChanged(const QString &)), this, SLOT(SlotEditTextChangedMood(const QString &)));

	connect(ui->lineEditTitle, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishTitle()));
	connect(ui->lineEditTempo, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishTempo()));
	connect(ui->lineEditYear, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishYear()));
	connect(ui->lineEditTrack, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishTrack()));
	connect(ui->lineEditPath, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishPath()));
	connect(ui->lineEditBitrate, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishBitrate()));
	connect(ui->lineEditDuration, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishDuration()));
	connect(ui->lineEditSampleRate, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishSampleRate()));
	connect(ui->lineEditChannel, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishChannel()));
	connect(ui->lineEditFormat, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishFormat()));
	connect(ui->lineEditCDYear, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDYear()));
	connect(ui->lineEditCDNumber, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDNumber()));
	connect(ui->lineEditCDTotal, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDTotal()));

}

void TrackInfo::Initialize()
{
	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();

	ui->cbAlbum->setEditable(true);
	ui->cbAlbumArtist->setEditable(true);
	ui->cbArtist->setEditable(true);
	ui->cbGenre->setEditable(true);
	ui->cbComposer->setEditable(true);
	ui->cbMood->setEditable(true);

	ui->frameTitle->hide();
	ui->frameAlbum->hide();
	ui->frameAlbumArtist->hide();
	ui->frameArtist->hide();
	ui->frameComposer->hide();
	ui->frameGenre->hide();
	ui->frameMood->hide();
	ui->frameTempo->hide();
	ui->frameYear->hide();
	ui->frameTrack->hide();
	ui->framePath->hide();
	ui->frameBitrate->hide();
	ui->frameDuration->hide();
	ui->frameSampleRate->hide();
	ui->frameChannel->hide();
	ui->frameFormat->hide();
	ui->frameCDYear->hide();
	ui->frameCDNumber->hide();
	ui->frameCDTotal->hide();

	SetMode(TRACK_INFO_MODE_VIEW);

}

void TrackInfo::SetAlbum(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameAlbum->show();
		ui->cbAlbum->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameAlbum->show();
		ui->cbAlbum->addItems(list);
		ui->cbAlbum->setCurrentIndex(0);
	}
}

void TrackInfo::SetAlbumArtist(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameAlbumArtist->show();
		ui->cbAlbumArtist->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameAlbumArtist->show();
		ui->cbAlbumArtist->addItems(list);
		ui->cbAlbumArtist->setCurrentIndex(0);
	}
}

void TrackInfo::SetArtist(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameArtist->show();
		ui->cbArtist->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameArtist->show();
		ui->cbArtist->addItems(list);
		ui->cbArtist->setCurrentIndex(0);
	}
}

void TrackInfo::SetComposer(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameComposer->show();
		ui->cbComposer->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameComposer->show();
		ui->cbComposer->addItems(list);
		ui->cbComposer->setCurrentIndex(0);
	}
}

void TrackInfo::SetGenre(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameGenre->show();
		ui->cbGenre->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameGenre->show();
		ui->cbGenre->addItems(list);
		ui->cbGenre->setCurrentIndex(0);
	}
}

void TrackInfo::SetMood(const QStringList &list, const QString value)
{
	if (!value.isEmpty())
	{
		ui->frameMood->show();
		ui->cbMood->addItem(value);
	}

	if (list.count() > 0)
	{
		ui->frameMood->show();
		ui->cbMood->addItems(list);
		ui->cbMood->setCurrentIndex(0);
	}
}

void TrackInfo::SetTitle(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameTitle->show();
		ui->lineEditTitle->setText(value);
	}
}

void TrackInfo::SetTempo(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameTempo->show();
		ui->lineEditTempo->setText(value);
	}
}

void TrackInfo::SetYear(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameYear->show();
		ui->lineEditYear->setText(value);

	}
}

void TrackInfo::SetTrack(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameTrack->show();
		ui->lineEditTrack->setText(value);

	}
}

void TrackInfo::SetPath(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->framePath->show();

	}
}

void TrackInfo::SetBitrate(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameBitrate->show();
		ui->lineEditBitrate->setText(value);

	}
}

void TrackInfo::SetDuration(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameDuration->show();
		ui->lineEditDuration->setText(value);

	}
}

void TrackInfo::SetSampleRate(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameSampleRate->show();
		ui->lineEditSampleRate->setText(value);

	}
}

void TrackInfo::SetChannel(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameChannel->show();
		ui->lineEditChannel->setText(value);

	}
}

void TrackInfo::SetFormat(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameFormat->show();
		ui->lineEditFormat->setText(value);

	}
}

void TrackInfo::SetCDYear(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameCDYear->show();
		ui->lineEditCDYear->setText(value);

	}
}

void TrackInfo::SetCDNumber(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameCDNumber->show();
		ui->lineEditCDNumber->setText(value);

	}
}

void TrackInfo::SetCDTotal(const QString &value)
{
	if (!value.isEmpty())
	{
		ui->frameCDTotal->show();
		ui->lineEditCDTotal->setText(value);

	}
}

int TrackInfo::GetMode() const
{
	return m_Mode;
}

void TrackInfo::SetMode(int Mode)
{
	m_Mode = Mode;
	if (m_Mode == TRACK_INFO_MODE_VIEW)
	{
		ui->cbAlbum->setEnabled(false);
		ui->cbAlbumArtist->setEnabled(false);
		ui->cbArtist->setEnabled(false);
		ui->cbComposer->setEnabled(false);
		ui->cbGenre->setEnabled(false);
		ui->cbMood->setEnabled(false);

		ui->lineEditTitle->setReadOnly(true);
		ui->lineEditTempo->setReadOnly(true);
		ui->lineEditYear->setReadOnly(true);
		ui->lineEditTrack->setReadOnly(true);
		ui->lineEditPath->setReadOnly(true);
		ui->lineEditBitrate->setReadOnly(true);
		ui->lineEditDuration->setReadOnly(true);
		ui->lineEditSampleRate->setReadOnly(true);
		ui->lineEditChannel->setReadOnly(true);
		ui->lineEditFormat->setReadOnly(true);
		ui->lineEditCDYear->setReadOnly(true);
		ui->lineEditCDNumber->setReadOnly(true);
		ui->lineEditCDTotal->setReadOnly(true);
	}
	else
	{
		ui->cbAlbum->setEnabled(true);
		ui->cbAlbumArtist->setEnabled(true);
		ui->cbArtist->setEnabled(true);
		ui->cbComposer->setEnabled(true);
		ui->cbGenre->setEnabled(true);
		ui->cbMood->setEnabled(true);

		ui->lineEditTitle->setReadOnly(false);
		ui->lineEditTempo->setReadOnly(false);
		ui->lineEditYear->setReadOnly(false);
		ui->lineEditTrack->setReadOnly(false);
		ui->lineEditPath->setReadOnly(false);
		ui->lineEditBitrate->setReadOnly(false);
		ui->lineEditDuration->setReadOnly(false);
		ui->lineEditSampleRate->setReadOnly(false);
		ui->lineEditChannel->setReadOnly(false);
		ui->lineEditFormat->setReadOnly(false);
		ui->lineEditCDYear->setReadOnly(false);
		ui->lineEditCDNumber->setReadOnly(false);
		ui->lineEditCDTotal->setReadOnly(false);
	}
}
