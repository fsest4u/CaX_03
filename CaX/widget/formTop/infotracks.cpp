#include <QMouseEvent>

#include "infotracks.h"
#include "ui_infotracks.h"

#include "widget/form/formcoverart.h"
#include "widget/form/formtitle.h"
#include "widget/form/formplay.h"
#include "widget/form/formsort.h"

#include "util/log.h"

InfoTracks::InfoTracks(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	m_pFormTitle(new FormTitle(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoTracks)
{
	ui->setupUi(this);

	ConnectSigToSlot();

}

InfoTracks::~InfoTracks()
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

	if (m_pFormPlay)
	{
		delete m_pFormPlay;
		m_pFormPlay = nullptr;
	}

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

	delete ui;

}

void InfoTracks::SetTitle(QString title)
{
	m_pFormTitle->SetTitleFontSize(42);
	m_pFormTitle->SetTitle(title);
}

void InfoTracks::SetSubtitle(QString subtitle)
{
	m_pFormTitle->SetSubtitleFontSize(20);
	m_pFormTitle->SetSubtitle(subtitle);
}

//void InfoTracks::SetInfo(QString info)
//{
//	if (info.isEmpty())
//	{
//		info = tr("-");
//	}
//	ui->labelInfo->setText(info);
//}

void InfoTracks::SetCoverArt(QString coverArt)
{
//	if (coverArt.isEmpty())
//	{
//		coverArt = ":/resource/Icon-playbar-volume-160.png";
//	}

//	QImage image;
//	if (image.load(coverArt))
//	{
////			painter->drawImage(coverRect, image);
//		QPixmap pixmap = QPixmap::fromImage(image);
//		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
//												   , ui->labelCoverArt->height()
//												   , Qt::KeepAspectRatio));
//	}
	m_pFormCoverArt->SetCoverArt(coverArt);
}

void InfoTracks::SlotCoverArt()
{
	LogDebug("good choice cover art");
}

void InfoTracks::SlotTitle()
{
	LogDebug("good choice title");

}

void InfoTracks::SlotSubtitle()
{
	LogDebug("good choice subtitle");

}

void InfoTracks::SlotPlayAll()
{
	LogDebug("good choice play all");

}

void InfoTracks::SlotPlayRandom()
{
	LogDebug("good choice play random");

}

void InfoTracks::SlotFavorite()
{
	LogDebug("good choice favorite");

}

void InfoTracks::SlotRating()
{
	LogDebug("good choice rating");

}

void InfoTracks::SlotSubmenu()
{
	LogDebug("good choice submenu");

}

void InfoTracks::SlotSort()
{
	LogDebug("good choice sort");

}

void InfoTracks::SlotIncDec()
{
	LogDebug("good choice inc dec");

}

void InfoTracks::SlotResize()
{
	LogDebug("good choice resize");

}

void InfoTracks::ConnectSigToSlot()
{

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
	ui->gridLayoutFormTitle->addWidget(m_pFormTitle);
	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	connect(m_pFormCoverArt, SIGNAL(SigCoverArt()), this, SLOT(SlotCoverArt()));
	connect(m_pFormTitle, SIGNAL(SigTitle()), this, SLOT(SlotTitle()));
	connect(m_pFormTitle, SIGNAL(SigSubtitle()), this, SLOT(SlotSubtitle()));
	connect(m_pFormPlay, SIGNAL(SigPlayAll()), this, SLOT(SlotPlayAll()));
	connect(m_pFormPlay, SIGNAL(SigPlayRandom()), this, SLOT(SlotPlayRandom()));
	connect(m_pFormPlay, SIGNAL(SigFavorite()), this, SLOT(SlotFavorite()));
	connect(m_pFormPlay, SIGNAL(SigRating()), this, SLOT(SlotRating()));
	connect(m_pFormPlay, SIGNAL(SigSubmenu()), this, SLOT(SlotSubmenu()));
	connect(m_pFormSort, SIGNAL(SigSort()), this, SLOT(SlotSort()));
	connect(m_pFormSort, SIGNAL(SigIncDec()), this, SLOT(SlotIncDec()));
	connect(m_pFormSort, SIGNAL(SigResize()), this, SLOT(SlotResize()));

}
