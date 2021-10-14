#ifndef INFOSERVICE_H
#define INFOSERVICE_H

#include <QWidget>

class SubmenuDialog;
class FormTitle;
class FormPlay;
class FormSort;

namespace Ui {
class InfoService;
}

class InfoService : public QWidget
{
	Q_OBJECT

public:
	explicit InfoService(QWidget *parent = nullptr);
	~InfoService();

	enum {
		ISERVICE_NONE = 0,
		ISERVICE_MAX
	};

	enum {
		FM_SEARCH_ALL_DELETE = 0,
		FM_SEARCH_ALL,
		FM_ADD,
		FM_DELETE,
		FM_RESERVE_LIST,
		FM_MAX
	};

	enum {
		DAB_SEARCH_ALL_DELETE = 0,
		DAB_SEARCH_ALL,
//		DAB_ADD,
		DAB_DELETE,
		DAB_RESERVE_LIST,
		DAB_MAX
	};

	void SetTitle(const QString title);
	void SetSubtitle(const QString title);

	void SetSubmenuIService();
	void SetSubmenuFmRadio();
	void SetSubmenuDabRadio();

	FormPlay *GetFormPlay();
	FormSort *GetFormSort();

private slots:

	void SlotTitle();
	void SlotSubtitle();


private:

	void ConnectSigToSlot();

	void ShowSubmenuDialog();

	SubmenuDialog	*m_pSubmenuDlg;
	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;
	FormSort		*m_pFormSort;

	Ui::InfoService *ui;
};

#endif // INFOSERVICE_H
