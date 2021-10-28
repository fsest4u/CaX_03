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

	void ShowPlayAll();
	void ShowPlayRandom();
	void ShowFavorite();
	void ShowRating();
	void ShowTopMenu();

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetRating() const;
	void SetRating(int Rating);

	void ClearTopMenu();
	void SetTopMenu(QMap<int, QString> map);


signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigFavorite(int Favorite);
	void SigRating(int Rating);
	void SigTopMenu();
	void SigTopMenuAction(int menuID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotBtnRating1();
	void SlotBtnRating2();
	void SlotBtnRating3();
	void SlotBtnRating4();
	void SlotBtnRating5();

	void SlotBtnTopMenu();
	void SlotTopMenuAction(QAction *action);

private:

	void ConnectSigToSlot();
	void Initialize();

	QIcon GetIcon(QString value);

	QMenu	*m_TopMenu;

	int m_Favorite;
	int m_Rating;

	Ui::FormPlay *ui;
};

#endif // FORMPLAY_H
