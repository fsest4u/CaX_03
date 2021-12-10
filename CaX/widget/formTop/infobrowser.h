#ifndef INFOBROWSER_H
#define INFOBROWSER_H

#include <QWidget>

class FormCoverArt;
//class FormTitle;
class FormPlay;

namespace Ui {
class InfoBrowser;
}

class InfoBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit InfoBrowser(QWidget *parent = nullptr);
	~InfoBrowser();

	void SetCoverArt(QString coverArt);
	QString GetTitle() const;
	void SetTitle(const QString title);
	QString GetSubtitle() const;
	void SetSubtitle(const QString title);

	FormPlay *GetFormPlay();

private slots:

	void SlotCoverArt();
	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
//	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;

	Ui::InfoBrowser *ui;
};

#endif // INFOBROWSER_H
