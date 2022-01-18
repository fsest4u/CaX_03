#ifndef INFOHOME_H
#define INFOHOME_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"

//class FormTitle;
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

	QString GetTitle() const;
	void SetTitle(const QString title);
	QString GetCategoryCnt() const;
	void SetCategoryCnt(const QString count);

	void SetAlbumCnt(const QString count);
	void SetAlbumArtistCnt(const QString count);
	void SetArtistCnt(const QString count);
	void SetGenreCnt(const QString count);

	FormPlay *GetFormPlay();
	FormSort *GetFormSort();
	FormClassify *GetFormClassify();

	void ClearMenu();
	void SetMenu(QMap<int, QString> list);

signals:

	void SigAlbumList();
	void SigAlbumArtistList();
	void SigArtistList();
	void SigGenreList();
	void SigMenu();
	void SigMenuAction(int nCategory, QString title);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenu();
	void SlotMenuAction(QAction *action);

private:

	void ConnectSigToSlot();
	void Initialize();

//	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;
	FormClassify	*m_pFormClassify;
	FormSort		*m_pFormSort;

	QMenu			*m_Menu;

	Ui::InfoHome *ui;
};

#endif // INFOHOME_H
