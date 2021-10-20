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

	FormCoverArt *GetFormCoverArt() const;
	FormTitle *GetFormTitle() const;

signals:

	void SigClickPlay(int nID);
	void SigClickFavorite(int nID, int nFavorite);
	void SigClickRating(int nID, int nRating);
	void SigClickTitle(int nID, QString coverArt);
	void SigClickSubtitle(int nID, QString coverArt);

private slots:

	void SlotRating(int nRating);
	void SlotFavorite(int nFavorite);
	void SlotPlay();
	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;

	int		m_ID;

	Ui::IconTracksEditor *ui;
};

#endif // ICONTRACKSEDITOR_H
