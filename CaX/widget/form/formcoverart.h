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

	bool GetMute() const;
	void SetMute(bool Mute);

	int GetIndex() const;
	void SetIndex(int Index);

	void SetMinimumSizeCheck(int size);

signals:

	void SigCoverArt(int index);
	void SigCheck(int index);
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

	void ConnectSigToSlot();
	void Initialize();

	QString m_CoverArt;
	int		m_Rating;
	int		m_Favorite;
	int		m_Count;
	int		m_Index;
	bool	m_Select;
	bool	m_Mute;

	Ui::FormCoverArt *ui;
};

#endif // FORMCOVERART_H
