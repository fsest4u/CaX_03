#include <QMouseEvent>

#include "infohome.h"
#include "ui_infohome.h"

#include "widget/form/formplay.h"
#include "widget/form/formsort.h"
#include "widget/form/formclassify.h"

#include "dialog/submenudialog.h"

#include "util/caxkeyvalue.h"
#include "util/log.h"

InfoHome::InfoHome(QWidget *parent)	:
	QWidget(parent),
	m_pCatDlg(new SubmenuDialog(this)),
	m_pSortDlg(new SubmenuDialog(this)),
	m_pFormPlay(new FormPlay(this)),
	m_pFormClassify(new FormClassify(this)),
	m_pFormSort(new FormSort(this)),
	ui(new Ui::InfoHome)
{
	ui->setupUi(this);

	ui->frameAlbum->installEventFilter(this);
	ui->frameArtist->installEventFilter(this);
	ui->frameTrack->installEventFilter(this);
	ui->frameGenre->installEventFilter(this);
	ui->frameSubmenu2->installEventFilter(this);

	ui->gridLayoutFormPlay->addWidget(m_pFormPlay);
	ui->gridLayoutFormClassify->addWidget(m_pFormClassify);
	ui->gridLayoutFormSort->addWidget(m_pFormSort);

	SetCategoryDialog();
	SetSortDialog();
}

InfoHome::~InfoHome()
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

	if (m_pFormPlay)
	{
		delete m_pFormPlay;
		m_pFormPlay = nullptr;
	}

	if (m_pFormClassify)
	{
		delete m_pFormClassify;
		m_pFormClassify = nullptr;
	}

	if (m_pFormSort)
	{
		delete m_pFormSort;
		m_pFormSort = nullptr;
	}

	delete ui;
}

void InfoHome::SetAlbumCnt(const QString count)
{
	ui->labelAlbumCnt->setText(count);
}

void InfoHome::SetArtistCnt(const QString count)
{
	ui->labelArtistCnt->setText(count);

}

void InfoHome::SetTrackCnt(const QString count)
{
	ui->labelTrackCnt->setText(count);

}

void InfoHome::SetGenreCnt(const QString count)
{
	ui->labelGenreCnt->setText(count);

}

void InfoHome::SetSortName(const QString name)
{
//	ui->labelSort->setText(name);

}

FormPlay *InfoHome::GetFormPlay()
{
	return m_pFormPlay;
}

FormClassify *InfoHome::GetFormClassify()
{
	return m_pFormClassify;
}

FormSort *InfoHome::GetFormSort()
{
	return m_pFormSort;
}

bool InfoHome::eventFilter(QObject *object, QEvent *event)
{

	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->frameGenre)
		{
			emit SigGenreList();
		}
		else if (object == ui->frameAlbum)
		{
			emit SigAlbumList();
		}
		else if (object == ui->frameArtist)
		{
			emit SigArtistList();
		}
		else if (object == ui->frameTrack)
		{
			emit SigTrackList();
		}
		else if (object == ui->frameSubmenu2)
		{
			emit SigSubmenu2();
		}

	}

	return QObject::eventFilter(object, event);
}




void InfoHome::SetCategoryDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		CATEGORY_GENRE,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR
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
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pCatDlg->SetItemList(list);
}

void InfoHome::SetSortDialog()
{
	QList<CJsonNode> list;

	QList<int> listID = {
		SORT_GENRE,
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_RATING,
		SORT_SAMPLE_RATE
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
		node.Add(KEY_COVER_ART, ":/resource/Icon-playbar-volume-160.png");
		node.Add(KEY_NAME, listName.at(i));
		list.append(node);
	}
	m_pSortDlg->SetItemList(list);
}


void InfoHome::ShowCategoryDialog()
{
//	if (m_pCatDlg->isHidden())
//	{
//		m_pCatDlg->move(mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).x() - m_pCatDlg->width()
//						, mapToGlobal(ui->frameSubmenu->geometry().bottomRight()).y());

//		if (m_pCatDlg->exec())
//		{
//			int id = m_pCatDlg->GetID();
//			LogDebug("sort type id [%d]", id);
//			// emit signal sort...
//		}
//	}
//	else
//	{
//		m_pCatDlg->close();
//	}
}

void InfoHome::ShowSortDialog()
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
