#ifndef FORMPLAY_H
#define FORMPLAY_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class FormPlay;
}

class FormPlay : public QWidget
{
	Q_OBJECT

public:
	explicit FormPlay(QWidget *parent = nullptr);
	~FormPlay();

	void ShowPlayAll(bool show = true);
	void ShowPlayRandom(bool show = true);
	void ShowFavorite(bool show = true);
	void ShowRating(bool show = true);
	void ShowMenu(bool show = true);

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetRating() const;
	void SetRating(int Rating);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);


signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigFavorite(int Favorite);
	void SigRating(int Rating);
	void SigMenu();
	void SigMenuAction(int menuID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotBtnRating0();
	void SlotBtnRating1();
	void SlotBtnRating2();
	void SlotBtnRating3();
	void SlotBtnRating4();
	void SlotBtnRating5();

	void SlotMenu();
	void SlotMenuAction(QAction *action);

private:

	void ConnectSigToSlot();
	void Initialize();

	QMenu	*m_Menu;

	int m_Favorite;
	int m_Rating;

	Ui::FormPlay *ui;
};

#endif // FORMPLAY_H
