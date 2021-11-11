#include "cdripinfo.h"
#include "ui_cdripinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

CDRipInfo::CDRipInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CDRipInfo)
{
	ui->setupUi(this);

	m_AlbumList.clear();
	m_AlbumArtistList.clear();

	connect(ui->cbFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedFormat(int)));
	connect(ui->cbAlbum, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedAlbum(int)));
	connect(ui->cbAlbumArtist, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangedAlbum(int)));
	connect(ui->lineEditCDYear, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDYear()));
	connect(ui->lineEditCDNumber, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDNumber()));
	connect(ui->lineEditCDTotal, SIGNAL(editingFinished()), this, SLOT(SlotEditFinishCDTotal()));
//	connect(ui->labelCoverArt, SIGNAL(), this, SLOT());
}

CDRipInfo::~CDRipInfo()
{
	delete ui;
}

CJsonNode CDRipInfo::GetInfoData()
{
	return m_InfoData;
}

void CDRipInfo::SetInfoData(CJsonNode node)
{
	m_InfoData = node;
	LogDebug("node [%s]", m_InfoData.ToCompactByteArray().data());
	ui->cbFormat->addItems(m_InfoData.GetStringList(KEY_FORMATS));
	ui->cbFormat->setCurrentIndex(m_InfoData.GetInt(KEY_FORMAT));

	if (!m_InfoData.GetString(KEY_ALBUM).isEmpty())
		ui->cbAlbum->addItem(m_InfoData.GetString(KEY_ALBUM));
	ui->cbAlbum->addItems(m_AlbumList);
	ui->cbAlbum->setCurrentIndex(0);

	if (!m_InfoData.GetString(KEY_ALBUM_ARTIST).isEmpty())
		ui->cbAlbumArtist->addItem(m_InfoData.GetString(KEY_ALBUM_ARTIST));
	ui->cbAlbumArtist->addItems(m_AlbumArtistList);
	ui->cbAlbumArtist->setCurrentIndex(0);

	ui->lineEditCDYear->setText(m_InfoData.GetString(KEY_CDYEAR));
	ui->lineEditCDNumber->setText(m_InfoData.GetString(KEY_CDNUMBER));
	ui->lineEditCDTotal->setText(m_InfoData.GetString(KEY_CDTOTAL));
	// cover art
}

void CDRipInfo::SlotChangedFormat(int index)
{
	emit SigChangeFormat(index);
}

void CDRipInfo::SlotChangedAlbum(int index)
{
	emit SigChangeAlbum(ui->cbAlbum->itemText(index));
}

void CDRipInfo::SlotChangedAlbumArtist(int index)
{
	emit SigChangeAlbumArtist(ui->cbAlbumArtist->itemText(index));
}

void CDRipInfo::SlotEditFinishCDYear()
{
	emit SigChangeCDYear(ui->lineEditCDYear->text());
}

void CDRipInfo::SlotEditFinishCDNumber()
{
	emit SigChangeCDNumber(ui->lineEditCDNumber->text());
}

void CDRipInfo::SlotEditFinishCDTotal()
{
	emit SigChangeCDTotal(ui->lineEditCDTotal->text());
}

QStringList CDRipInfo::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void CDRipInfo::SetAlbumArtistList(const QStringList &AlbumArtistList)
{
	m_AlbumArtistList = AlbumArtistList;
}

QStringList CDRipInfo::GetAlbumList() const
{
	return m_AlbumList;
}

void CDRipInfo::SetAlbumList(const QStringList &AlbumList)
{
	m_AlbumList = AlbumList;
}
