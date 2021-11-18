#include "trackinfodialog.h"
#include "ui_trackinfodialog.h"

#include "dialog/trackinfo.h"

#include "util/log.h"


TrackInfoDialog::TrackInfoDialog(QWidget *parent) :
	QDialog(parent),
	m_TrackInfo(new TrackInfo()),
	ui(new Ui::TrackInfoDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	ui->gridLayoutScrollArea->addWidget(m_TrackInfo);

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
}

void TrackInfoDialog::SlotChangeAlbum(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeAlbumArtist(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeArtist(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeComposer(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeGenre(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeMood(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeTitle(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeTempo(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeYear(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeTrack(int index, QString value)
{

}

void TrackInfoDialog::SlotChangePath(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeBitrate(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeDuration(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeSampleRate(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeChannel(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeFormat(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeCDYear(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeCDNumber(int index, QString value)
{

}

void TrackInfoDialog::SlotChangeCDTotal(int index, QString value)
{

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

}

CJsonNode TrackInfoDialog::GetInfoData() const
{
	return m_InfoData;
}

void TrackInfoDialog::SetInfoData(const CJsonNode &InfoData)
{
	m_InfoData = InfoData;
	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());

	m_TrackInfo->SetInfoData(m_InfoData);

}
