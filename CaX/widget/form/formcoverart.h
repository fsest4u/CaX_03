#ifndef FORMCOVERART_H
#define FORMCOVERART_H

#include <QWidget>

namespace Ui {
class FormCoverArt;
}

class FormCoverArt : public QWidget
{
	Q_OBJECT

public:
	explicit FormCoverArt(QWidget *parent = nullptr);
	~FormCoverArt();

	QString GetCoverArt();
	void SetCoverArt(QString coverArt);

	int GetRating() const;
	void SetRating(int Rating = -1);

	int GetFavorite() const;
	void SetFavorite(int Favorite = -1);

	int GetCount() const;
	void SetCount(int Count);

	bool GetSelect() const;
	void SetSelect(bool Select);

signals:

	void SigRating(int nRating);
	void SigFavorite(int nFavorite);
	void SigPlay();


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotBtnRating1();
	void SlotBtnRating2();
	void SlotBtnRating3();
	void SlotBtnRating4();
	void SlotBtnRating5();

private:

	QString m_CoverArt;
	int		m_Rating;
	int		m_Favorite;
	int		m_Count;
	bool	m_Select;

	Ui::FormCoverArt *ui;
};

#endif // FORMCOVERART_H
