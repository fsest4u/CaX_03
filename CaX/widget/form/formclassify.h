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

signals:

	void SigClassify();
	void SigFavorite();
	void SigRating();
	void SigClassifyArtist(bool bAdd, QString id);
	void SigClassifyGenre(bool bAdd, QString id);
	void SigClassifyComposer(bool bAdd, QString id);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotArtistMenu(QAction* action);
	void SlotGenreMenu(QAction* action);
	void SlotComposerMenu(QAction* action);

private:

	void SetClassifyMenu();


	QMenu		*m_ClassifyMenu;
	QMenu		*m_GenreMenu;
	QMenu		*m_ArtistMenu;
	QMenu		*m_ComposerMenu;

	Ui::FormClassify *ui;
};

#endif // FORMCLASSIFY_H
