#ifndef ICONTRACKSEDITOR_H
#define ICONTRACKSEDITOR_H

#include <QWidget>

class FormCoverArt;
class FormTitle;

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
	void SetCoverArt(const QString &coverArt);

	QString GetTitle();
	void SetTitle(const QString &title);

	QString GetSubtitle();
	void SetSubtitle(const QString &subtitle);

//	QString GetCount();
//	void SetCount(const QString &value);

//	int GetFavorite() const;
//	void SetFavorite(const int &value);

//	int GetRating() const;
//	void SetRating(const int &value);

signals:

	void SigClickCoverArt(int id, QString coverArt);
	void SigClickTitle(int nID);
	void SigClickSubtitle(int nID);
	void SigClickCount(int nID);
	void SigClickFavorite(int nID, int nFavorite);
	void SigClickRating(int nID, int nRating);


protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotCoverArt();
	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;

	int		m_ID;
	QString m_CoverArt;


	Ui::IconTracksEditor *ui;
};

#endif // ICONTRACKSEDITOR_H
