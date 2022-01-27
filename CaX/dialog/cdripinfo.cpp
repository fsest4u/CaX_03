#include <QMouseEvent>

#include "cdripinfo.h"
#include "ui_cdripinfo.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "dialog/searchcoverartdialog.h"
#include "dialog/searchcoverartresultdialog.h"

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
//	connect(ui->btnCoverArt, SIGNAL(clicked()), this, SLOT(SlotClickCoverArt()));

	ui->labelCoverArt->installEventFilter(this);

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

void CDRipInfo::SlotClickCoverArt()
{
	QString site;
	QString keyword;
	QString artist;

	SearchCoverArtDialog searchDialog;
	searchDialog.SetKeyWord(ui->cbAlbum->currentText());
	searchDialog.SetArtist(ui->cbAlbumArtist->currentText());
	if (searchDialog.exec() == QDialog::Accepted)
	{
		site = searchDialog.GetSite();
		keyword = searchDialog.GetKeyword();
		artist = searchDialog.GetArtist();
	}
	else
	{
		return;
	}

	SearchCoverArtResultDialog resultDialog;
	resultDialog.SetAddr(m_Addr);
	resultDialog.RequestCoverArtList(site, keyword, artist);
	if (resultDialog.exec() == QDialog::Accepted)
	{
#if 0
		QString style;
		style = QString("QLabel	\
						{	\
						  border-image: url(\'\');	\
						}");
		ui->labelCoverArt->setStyleSheet(style);

		QPixmap *pixmap = new QPixmap();
		pixmap->loadFromData(resultDialog.GetImageData());
		ui->labelCoverArt->setPixmap(pixmap->scaled(ui->labelCoverArt->width()
													, ui->labelCoverArt->height()
													, Qt::KeepAspectRatio));

#else
		QString style;
		style = QString("QLabel	\
						{	\
						  border-image: url(\'%1\');	\
						}").arg(resultDialog.GetImagePath());
		ui->labelCoverArt->setStyleSheet(style);
#endif

		emit SigChangeCoverArt(resultDialog.GetImage(), resultDialog.GetThumb());
	}
}

QString CDRipInfo::GetImage() const
{
	return m_Image;
}

void CDRipInfo::SetImage(const QString &Image)
{
	m_Image = Image;
}

QString CDRipInfo::GetThumb() const
{
	return m_Thumb;
}

void CDRipInfo::SetThumb(const QString &Thumb)
{
	m_Thumb = Thumb;
}

QString CDRipInfo::GetCoverArt() const
{
	return "";
}

void CDRipInfo::SetCoverArt(const QString &CoverArt)
{
	QString style;
	style = QString("QLabel	\
					{	\
					  border-image: url(\'%1\');	\
					}").arg(CoverArt);
	ui->labelCoverArt->setStyleSheet(style);
}

bool CDRipInfo::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (object == ui->labelCoverArt)
			{
				SlotClickCoverArt();
			}
		}
	}

	return QObject::eventFilter(object, event);
}

QString CDRipInfo::GetAddr() const
{
	return m_Addr;
}

void CDRipInfo::SetAddr(const QString &Addr)
{
	m_Addr = Addr;
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
