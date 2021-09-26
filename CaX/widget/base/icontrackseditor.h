#ifndef ICONTRACKSEDITOR_H
#define ICONTRACKSEDITOR_H

#include <QWidget>

namespace Ui {
class IconTracksEditor;
}

class IconTracksEditor : public QWidget
{
	Q_OBJECT

public:
	explicit IconTracksEditor(QWidget *parent = nullptr);
	~IconTracksEditor();

	int GetID();
	void SetID(int nID);

	QString GetCoverArt();
	void SetCoverArt(const QString &value);

	QString GetTop();
	void SetTop(const QString &value);

	QString GetBottom();
	void SetBottom(const QString &value);

	QString GetCount();
	void SetCount(const QString &value);

	int GetFavorite() const;
	void SetFavorite(const int &value);

	int GetRating() const;
	void SetRating(const int &value);

signals:

	void SigClickCoverArt(int nID);
	void SigClickTop(int nID);
	void SigClickBottom(int nID);
	void SigClickCount(int nID);
	void SigClickFavorite(int nID, int nFavorite);
	void SigClickRating(int nID, int nRating);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	QString m_CoverArt;


	Ui::IconTracksEditor *ui;
};

#endif // ICONTRACKSEDITOR_H
