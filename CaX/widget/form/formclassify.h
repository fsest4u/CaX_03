#ifndef FORMCLASSIFY_H
#define FORMCLASSIFY_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"


namespace Ui {
class FormClassify;
}

class FormClassify : public QWidget
{
	Q_OBJECT

public:
	explicit FormClassify(QWidget *parent = nullptr);
	~FormClassify();

	void ClearClassifyArtistMenu();
	void SetClassifyArtistMenu(QList<CJsonNode> list);
	void ClearClassifyGenreMenu();
	void SetClassifyGenreMenu(QList<CJsonNode> list);
	void ClearClassifyComposerMenu();
	void SetClassifyComposerMenu(QList<CJsonNode> list);

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetRating() const;
	void SetRating(int Rating);

signals:

	//	void SigClassify();
	void SigFavorite(int nFavorite);
	void SigRating(int nRating);
	void SigClassifyArtist(bool bAdd, QString id);
	void SigClassifyGenre(bool bAdd, QString id);
	void SigClassifyComposer(bool bAdd, QString id);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotArtistMenu(QAction* action);
	void SlotGenreMenu(QAction* action);
	void SlotComposerMenu(QAction* action);

	void SlotBtnRating1();
	void SlotBtnRating2();
	void SlotBtnRating3();
	void SlotBtnRating4();
	void SlotBtnRating5();

private:

	void SetClassifyMenu();


	QMenu		*m_ClassifyMenu;
	QMenu		*m_GenreMenu;
	QMenu		*m_ArtistMenu;
	QMenu		*m_ComposerMenu;

	int			m_Favorite;
	int			m_Rating;

	Ui::FormClassify *ui;
};

#endif // FORMCLASSIFY_H
