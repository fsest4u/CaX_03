#include <QMouseEvent>

#include "musicinfo.h"
#include "ui_musicinfo.h"

#include "dialog/submenudialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

MusicInfo::MusicInfo(QWidget *parent)
	: QWidget(parent)
	, m_pCatDlg(new SubmenuDialog(this))
	, m_pSortDlg(new SubmenuDialog(this))
	, ui(new Ui::MusicInfo)
{
	ui->setupUi(this);

	ui->framePlayAll->installEventFilter(this);
	ui->framePlayRandom->installEventFilter(this);
	ui->frameAlbum->installEventFilter(this);
	ui->frameArtist->installEventFilter(this);
	ui->frameTrack->installEventFilter(this);
	ui->frameComposer->installEventFilter(this);
	ui->frameSubmenu->installEventFilter(this);
	ui->frameSubmenu2->installEventFilter(this);
	ui->frameDisplayMode->installEventFilter(this);
	ui->frameSort->installEventFilter(this);
	ui->frameSort2->installEventFilter(this);

	SetCategoryDialog();
	SetSortDialog();
}

MusicInfo::~MusicInfo()
{
	if (m_pCatDlg)
	{
		delete m_pCatDlg;
		m_pCatDlg = nullptr;
	}
	if (m_pSortDlg)
	{
		delete m_pSortDlg;
		m_pSortDlg = nullptr;
	}

	delete ui;
}

void MusicInfo::SetAlbumCnt(const QString count)
{
	ui->labelAlbumCnt->setText(count);
}

void MusicInfo::SetArtistCnt(const QString count)
{
	ui->labelArtistCnt->setText(count);

}

void MusicInfo::SetTrackCnt(const QString count)
{
	ui->labelTrackCnt->setText(count);

}

void MusicInfo::SetComposerCnt(const QString count)
{
	ui->labelComposerCnt->setText(count);

}

void MusicInfo::SetSortName(const QString name)
{
	ui->labelSort->setText(name);

}

bool MusicInfo::eventFilter(QObject *object, QEvent *event)
{

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->framePlayAll)
		{
			emit SigPlayAll();
		}
		else if (object == ui->framePlayRandom)
		{
			emit SigPlayRandom();
		}
		else if (object == ui->frameAlbum)
		{
			emit SigAlbum();
		}
		else if (object == ui->frameArtist)
		{
			emit SigArtist();
		}
		else if (object == ui->frameTrack)
		{
			emit SigTrack();
		}
		else if (object == ui->frameComposer)
		{
			emit SigComposer();
		}
		else if (object == ui->frameSubmenu)
		{
			ShowCategoryDialog();
		}
		else if (object == ui->frameSubmenu2)
		{
			emit SigSubmenu2();
		}
		else if (object == ui->frameDisplayMode)
		{
			emit SigDisplayMode();
		}
		else if (object == ui->frameSort)
		{
			ShowSortDialog();
		}
		else if (object == ui->frameSort2)
		{
			ShowSortDialog();
		}
	}

	return QObject::eventFilter(object, event);
}


void MusicInfo::SetCategoryDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		CAT_GENRE,
		CAT_MOOD,
		CAT_FOLDER,
		CAT_YEAR
	};
	QList<QString> listName = {
		"Sorted by Genre",
		"Sorted by Mood",
		"Sorted by Folder",
		"Sorted by Year"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pCatDlg->SetItemList(list);
}

void MusicInfo::SetSortDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		SORT_BY_GENRE,
		SORT_BY_MOOD,
		SORT_BY_FOLDER,
		SORT_BY_YEAR,
		SORT_BY_RATING,
		SORT_BY_SAMPLE_RATE
	};
	QList<QString> listName = {
		"Sorted by Genre",
		"Sorted by Mood",
		"Sorted by Folder",
		"Sorted by Year",
		"Sorted by Rating",
		"Sorted by Sample rate"
	};

	CJsonNode node(JSON_OBJECT);
	for (int i = 0; i < listID.count(); i++)
	{
		node.AddInt(KEY_ID_UPPER, listID.at(i));
		node.Add(KEY_COVER_ART, ":/resource/baseline_menu_black_24dp.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSortDlg->SetItemList(list);
}

void MusicInfo::ShowCategoryDialog()
{
	if (m_pCatDlg->isHidden())
	{
		m_pCatDlg->move(mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).x() - m_pCatDlg->width()
						, mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).y());

		if (m_pCatDlg->exec())
		{
			int id = m_pCatDlg->GetID();
			LogDebug("sort type id [%d]", id);
			// emit signal sort...
		}
	}
	else
	{
		m_pCatDlg->close();
	}
}

void MusicInfo::ShowSortDialog()
{
	if (m_pSortDlg->isHidden())
	{
		m_pSortDlg->move(mapToGlobal(this->geometry().bottomRight()).x() - m_pSortDlg->width()
						 , mapToGlobal(this->geometry().bottomRight()).y());

		if (m_pSortDlg->exec())
		{
			int id = m_pSortDlg->GetID();
			LogDebug("sort type id [%d]", id);
			// emit signal sort...
		}
	}
	else
	{
		m_pSortDlg->close();
	}
}
