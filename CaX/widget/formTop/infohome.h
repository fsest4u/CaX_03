#ifndef INFOHOME_H
#define INFOHOME_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"

class FormPlay;
class FormClassify;
class FormSort;

namespace Ui {
class InfoHome;
}

class InfoHome : public QWidget
{
	Q_OBJECT

public:
	explicit InfoHome(QWidget *parent = nullptr);
	~InfoHome();

	void SetTitle(const int nCategory);
	void SetAlbumCnt(const QString count);
	void SetArtistCnt(const QString count);
	void SetTrackCnt(const QString count);
	void SetGenreCnt(const QString count);

	FormPlay *GetFormPlay();
	FormSort *GetFormSort();
	FormClassify *GetFormClassify();

	void ClearCategoryMenu();
	void SetCategoryMenu(QMap<int, QString> list);

signals:

	void SigGenreList();
	void SigAlbumList();
	void SigArtistList();
	void SigTrackList();
	void SigCategoryMenu();
	void SigCategoryMenuAction(int nCategory, QString title);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotBtnCategoryMenu();
	void SlotCategoryMenuAction(QAction *action);

private:

	enum {
		CATEGORY_GENRE = 0,
		CATEGORY_MOOD,
		CATEGORY_FOLDER,
		CATEGORY_YEAR,
		CATEGORY_MAX
	};

	enum {
		SORT_GENRE = 0,
		SORT_MOOD,
		SORT_FOLDER,
		SORT_YEAR,
		SORT_RATING,
		SORT_SAMPLE_RATE,
		SORT_MAX
	};

	void Initialize();

	FormPlay		*m_pFormPlay;
	FormClassify	*m_pFormClassify;
	FormSort		*m_pFormSort;

	QMenu			*m_CategoryMenu;

	Ui::InfoHome *ui;
};

#endif // INFOHOME_H
