#ifndef INFOTRACKS_H
#define INFOTRACKS_H

#include <QWidget>

class FormCoverArt;
//class FormTitle;
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

	void SetCoverArt(QString coverArt);
	void SetTitle(QString title);
	void SetSubtitle(QString title);
//	void SetInfo(QString info);

	FormPlay *GetFormPlay();
	FormSort *GetFormSort();

private slots:

	void SlotCoverArt();
	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
//	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;
	FormSort		*m_pFormSort;

	Ui::InfoTracks *ui;
};

#endif // INFOTRACKS_H
