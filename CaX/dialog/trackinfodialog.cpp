#include "trackinfodialog.h"
#include "ui_trackinfodialog.h"

#include "dialog/trackinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"


TrackInfoDialog::TrackInfoDialog(QWidget *parent) :
	QDialog(parent),
	m_TrackInfo(new TrackInfo()),
	ui(new Ui::TrackInfoDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	Initialize();

}

TrackInfoDialog::~TrackInfoDialog()
{
	if (m_TrackInfo)
	{
		delete m_TrackInfo;
		m_TrackInfo = nullptr;
	}

	delete ui;
}

QString TrackInfoDialog::GetAddr() const
{
	return m_Addr;
}

void TrackInfoDialog::SetAddr(const QString &Addr)
{
	m_Addr = Addr;
}

int TrackInfoDialog::GetMode() const
{
	return m_TrackInfo->GetMode();
}

void TrackInfoDialog::SetMode(int Mode)
{
	m_TrackInfo->SetMode(Mode);
	if (Mode == TrackInfo::TRACK_INFO_MODE_EDIT)
	{
		ui->btnSave->show();
	}
}

void TrackInfoDialog::SetWindowTitle(QString title)
{
	setWindowTitle(title);
}

void TrackInfoDialog::SlotChangeAlbum(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_ALBUM, value);
}

void TrackInfoDialog::SlotChangeAlbumArtist(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_ALBUM_ARTIST, value);
}

void TrackInfoDialog::SlotChangeArtist(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_ARTIST, value);

}

void TrackInfoDialog::SlotChangeComposer(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_COMPOSER, value);

}

void TrackInfoDialog::SlotChangeGenre(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_GENRE, value);

}

void TrackInfoDialog::SlotChangeMood(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_MOOD, value);

}

void TrackInfoDialog::SlotChangeTitle(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_TITLE_CAP, value);

}

void TrackInfoDialog::SlotChangeTempo(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_TEMPO, value);

}

void TrackInfoDialog::SlotChangeYear(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_YEAR, value);

}

void TrackInfoDialog::SlotChangeTrack(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_TRACK, value);

}

void TrackInfoDialog::SlotChangePath(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_PATH, value);

}

void TrackInfoDialog::SlotChangeBitrate(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_FORMAT, value);

}

void TrackInfoDialog::SlotChangeDuration(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_DURATION, value);

}

void TrackInfoDialog::SlotChangeSampleRate(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_SAMPLERATE, value);

}

void TrackInfoDialog::SlotChangeChannel(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_CHANNEL, value);

}

void TrackInfoDialog::SlotChangeFormat(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_FORMAT, value);

}

void TrackInfoDialog::SlotChangeCDYear(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_CDYEAR, value);

}

void TrackInfoDialog::SlotChangeCDNumber(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_CDNUMBER, value);

}

void TrackInfoDialog::SlotChangeCDTotal(int index, QString value)
{
	Q_UNUSED(index)
	m_InfoData.Add(KEY_CDTOTAL, value);

}

void TrackInfoDialog::SlotClickEdit()
{
	SetBtnEdit(false);
	SetMode(TrackInfo::TRACK_INFO_MODE_EDIT);
}

void TrackInfoDialog::SlotClickSave()
{
	emit accept();
}

void TrackInfoDialog::SlotClickCancel()
{
	this->close();
}

void TrackInfoDialog::ConnectSigToSlot()
{
	connect(m_TrackInfo, SIGNAL(SigChangeAlbum(int, QString)), this, SLOT(SlotChangeAlbum(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeAlbumArtist(int, QString)), this, SLOT(SlotChangeAlbumArtist(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeArtist(int, QString)), this, SLOT(SlotChangeArtist(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeGenre(int, QString)), this, SLOT(SlotChangeGenre(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeComposer(int, QString)), this, SLOT(SlotChangeComposer(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeMood(int, QString)), this, SLOT(SlotChangeMood(int, QString)));

	connect(m_TrackInfo, SIGNAL(SigChangeTitle(int, QString)), this, SLOT(SlotChangeTitle(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeTempo(int, QString)), this, SLOT(SlotChangeTempo(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeYear(int, QString)), this, SLOT(SlotChangeYear(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeTrack(int, QString)), this, SLOT(SlotChangeTrack(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangePath(int, QString)), this, SLOT(SlotChangePath(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeBitrate(int, QString)), this, SLOT(SlotChangeBitrate(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeDuration(int, QString)), this, SLOT(SlotChangeDuration(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeSampleRate(int, QString)), this, SLOT(SlotChangeSampleRate(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeChannel(int, QString)), this, SLOT(SlotChangeChannel(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeFormat(int, QString)), this, SLOT(SlotChangeFormat(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeCDYear(int, QString)), this, SLOT(SlotChangeCDYear(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeCDNumber(int, QString)), this, SLOT(SlotChangeCDNumber(int, QString)));
	connect(m_TrackInfo, SIGNAL(SigChangeCDTotal(int, QString)), this, SLOT(SlotChangeCDTotal(int, QString)));

	connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(SlotClickEdit()));
	connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(SlotClickSave()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(SlotClickCancel()));

}

void TrackInfoDialog::Initialize()
{
	ui->gridLayoutScrollArea->addWidget(m_TrackInfo);

	ui->btnEdit->hide();
	ui->btnSave->hide();
	ui->btnCancel->show();
}

CJsonNode TrackInfoDialog::GetInfoData() const
{
	return m_InfoData;
}

void TrackInfoDialog::SetInfoData(const CJsonNode &InfoData)
{
	m_InfoData = InfoData;
	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());

	m_TrackInfo->SetAlbumList(m_AlbumList);
	m_TrackInfo->SetAlbumArtistList(m_AlbumArtistList);
	m_TrackInfo->SetArtistList(m_ArtistList);
	m_TrackInfo->SetGenreList(m_GenreList);
	m_TrackInfo->SetComposerList(m_ComposerList);
	m_TrackInfo->SetMoodList(m_MoodList);

	m_TrackInfo->SetInfoData(m_InfoData);

}

QStringList TrackInfoDialog::GetMoodList() const
{
	return m_MoodList;
}

void TrackInfoDialog::SetMoodList(const QStringList &MoodList)
{
	m_MoodList = MoodList;
}

void TrackInfoDialog::SetBtnEdit(bool show)
{
	if (show)
	{
		ui->btnEdit->show();
		ui->btnSave->hide();
	}
	else
	{
		ui->btnEdit->hide();
		ui->btnSave->show();
	}
}

void TrackInfoDialog::SetBtnCancel(bool show)
{
	if (show)
	{
		ui->btnCancel->show();
	}
	else
	{
		ui->btnCancel->hide();
	}
}

QStringList TrackInfoDialog::GetComposerList() const
{
	return m_ComposerList;
}

void TrackInfoDialog::SetComposerList(const QStringList &ComposerList)
{
	m_ComposerList = ComposerList;
}

QStringList TrackInfoDialog::GetGenreList() const
{
	return m_GenreList;
}

void TrackInfoDialog::SetGenreList(const QStringList &GenreList)
{
	m_GenreList = GenreList;
}

QStringList TrackInfoDialog::GetArtistList() const
{
	return m_ArtistList;
}

void TrackInfoDialog::SetArtistList(const QStringList &ArtistList)
{
	m_ArtistList = ArtistList;
}

QStringList TrackInfoDialog::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void TrackInfoDialog::SetAlbumArtistList(const QStringList &AlbumArtistList)
{
	m_AlbumArtistList = AlbumArtistList;
}

QStringList TrackInfoDialog::GetAlbumList() const
{
	return m_AlbumList;
}

void TrackInfoDialog::SetAlbumList(const QStringList &AlbumList)
{
	m_AlbumList = AlbumList;
}
