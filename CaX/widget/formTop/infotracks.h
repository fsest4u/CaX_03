#ifndef INFOTRACKS_H
#define INFOTRACKS_H

#include <QWidget>

class FormCoverArt;
class FormTitle;
class FormPlay;
class FormSort;

namespace Ui {
class InfoTracks;
}

class InfoTracks : public QWidget
{
	Q_OBJECT

public:
	explicit InfoTracks(QWidget *parent = nullptr);
	~InfoTracks();

	void SetTitle(QString title);
	void SetSubtitle(QString subtitle);
//	void SetInfo(QString info);
	void SetCoverArt(QString coverArt);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigSubmenu();
	void SigSort();

private slots:

	void SlotCoverArt();
	void SlotTitle();
	void SlotSubtitle();
	void SlotPlayAll();
	void SlotPlayRandom();
	void SlotFavorite();
	void SlotRating();
	void SlotSubmenu();
	void SlotSort();
	void SlotIncDec();
	void SlotResize();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;
	FormSort		*m_pFormSort;

	Ui::InfoTracks *ui;
};

#endif // INFOTRACKS_H
