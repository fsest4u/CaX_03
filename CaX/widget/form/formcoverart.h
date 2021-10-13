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
	void SetRating(int Rating);

	int GetFavorite() const;
	void SetFavorite(int Favorite);

	int GetCount() const;
	void SetCount(int Count);

signals:

	void SigCoverArt();
	void SigRating();
	void SigFavorite();
	void SigPlay();


protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	QString m_CoverArt;
	int		m_Rating;
	int		m_Favorite;
	int		m_Count;

	Ui::FormCoverArt *ui;
};

#endif // FORMCOVERART_H
