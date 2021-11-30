#include "searchcoverartresultdialog.h"
#include "ui_searchcoverartresultdialog.h"

#include "manager/searchcoverartresultmanager.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

#include "widget/formBottom/iconcoverart.h"
#include "widget/formBottom/iconcoverartdelegate.h"

SearchCoverArtResultDialog::SearchCoverArtResultDialog(QWidget *parent, const QString &addr) :
	QDialog(parent),
	m_pMgr(new SearchCoverArtResultManager(this)),
	m_pIconCoverArt(new IconCoverArt(this)),
	m_NextUrl(""),
	ui(new Ui::SearchCoverArtResultDialog)
{
	ui->setupUi(this);

	m_pMgr->SetAddr(addr);

	ConnectSigToSlot();
	Initialize();

}

SearchCoverArtResultDialog::~SearchCoverArtResultDialog()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	if (m_pIconCoverArt)
	{
		delete m_pIconCoverArt;
		m_pIconCoverArt = nullptr;
	}

	delete ui;
}

void SearchCoverArtResultDialog::RequestCoverArtList(QString site, QString keyword, QString artist)
{
	m_Site = site;
	m_Keyword = keyword;
	m_Artist = artist;

	m_pMgr->RequestCoverArtList(m_Site, m_Keyword, m_Artist);
}

void SearchCoverArtResultDialog::SetAddr(const QString &addr)
{
	m_pMgr->SetAddr(addr);
}


QString SearchCoverArtResultDialog::GetThumb() const
{
	return m_Thumb;
}

void SearchCoverArtResultDialog::SetThumb(const QString &Thumb)
{
	m_Thumb = Thumb;
}


QString SearchCoverArtResultDialog::GetImage() const
{
	return m_Image;
}

void SearchCoverArtResultDialog::SetImage(const QString &Image)
{
	m_Image = Image;
}

void SearchCoverArtResultDialog::SlotRespCoverArtList(CJsonNode node)
{
	CJsonNode urls = node.GetArray(VAL_URLS);
	if (urls.ArraySize() <= 0)
	{
		return;
	}

	QList<CJsonNode> curNodeList;
	for (int i = 0; i < urls.ArraySize(); i++)
	{
		m_NodeList.append(urls.GetArrayAt(i));
		curNodeList.append(urls.GetArrayAt(i));
	}

	m_NextUrl = node.GetString(VAL_NEXT_URL);

	m_pIconCoverArt->SetNodeList(curNodeList, IconCoverArt::ICON_COVER_ART_MUSIC_DB);
}

//void SearchCoverArtResultDialog::SlotRespSearchCoverArt(QByteArray data, int index)
//{
//	QStandardItem *itemIcon = m_pIconCoverArt->GetModel()->item(index);
//	itemIcon->setData(data, IconCoverArtDelegate::ICON_COVER_ART_COVER);
//	m_pIconCoverArt->GetModel()->setItem(index, itemIcon);
//}

void SearchCoverArtResultDialog::SlotRespSearchCoverArt(QString filename, int index)
{
	QStandardItem *itemIcon = m_pIconCoverArt->GetModel()->item(index);
	itemIcon->setData(filename, IconCoverArtDelegate::ICON_COVER_ART_COVER);
	m_pIconCoverArt->GetModel()->setItem(index, itemIcon);
}

void SearchCoverArtResultDialog::SlotSearchCoverArt(int index, QString url)
{
	m_pMgr->RequestSearchCoverArt(url, index);
}

void SearchCoverArtResultDialog::SlotAppendIconList()
{
	if (!m_NextUrl.isEmpty())
	{
		LogDebug("next url [%s]", m_NextUrl.toUtf8().data());
		m_pMgr->RequestCoverArtList(m_Site, m_Keyword, m_Artist, m_NextUrl);
	}
}

void SearchCoverArtResultDialog::SlotSelectCoverArt(int index)
{
	CJsonNode node = m_NodeList.at(index);
	SetImage(node.GetString(VAL_IMAGE));
	SetThumb(node.GetString(VAL_THUMB));

	QStandardItem *itemIcon = m_pIconCoverArt->GetModel()->item(index);
	SetImageData(qvariant_cast<QByteArray>(itemIcon->data(IconCoverArtDelegate::ICON_COVER_ART_COVER)));
//	SetImagePath(qvariant_cast<QString>(itemIcon->data(IconCoverArtDelegate::ICON_COVER_ART_COVER)));

	done(1);
}

void SearchCoverArtResultDialog::ConnectSigToSlot()
{
	connect(m_pMgr, SIGNAL(SigRespCoverArtList(CJsonNode)), this, SLOT(SlotRespCoverArtList(CJsonNode)));
//	connect(m_pMgr, SIGNAL(SigRespSearchCoverArt(QByteArray, int)), this, SLOT(SlotRespSearchCoverArt(QByteArray, int)));
	connect(m_pMgr, SIGNAL(SigRespSearchCoverArt(QString, int)), this, SLOT(SlotRespSearchCoverArt(QString, int)));

	connect(m_pIconCoverArt, SIGNAL(SigSearchCoverArt(int, QString)), this, SLOT(SlotSearchCoverArt(int, QString)));
	connect(m_pIconCoverArt, SIGNAL(SigAppendIconList()), this, SLOT(SlotAppendIconList()));
	connect(m_pIconCoverArt->GetDelegate(), SIGNAL(SigSelectCoverArt(int)), this, SLOT(SlotSelectCoverArt(int)));

	connect(ui->btnMore, SIGNAL(clicked()), this, SLOT(SlotAppendIconList()));
}

void SearchCoverArtResultDialog::Initialize()
{
	ui->gridLayoutCoverArt->addWidget(m_pIconCoverArt);

	m_pIconCoverArt->ClearNodeList();
	m_NodeList.clear();

}

QByteArray SearchCoverArtResultDialog::GetImageData() const
{
	return m_ImageData;
}

void SearchCoverArtResultDialog::SetImageData(const QByteArray &ImageData)
{
	m_ImageData = ImageData;
}

//QString SearchCoverArtResultDialog::GetImagePath() const
//{
//	return m_ImagePath;
//}

//void SearchCoverArtResultDialog::SetImagePath(const QString &ImagePath)
//{
//	m_ImagePath = ImagePath;
//}
