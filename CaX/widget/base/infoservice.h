#ifndef INFOSERVICE_H
#define INFOSERVICE_H

#include <QWidget>

namespace Ui {
class InfoService;
}

class SubmenuDialog;

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

	void SetSubmenuIService();
	void SetSubmenuFmRadio();
	void SetSubmenuDabRadio();


protected:

	bool eventFilter(QObject *object, QEvent *event);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigSubmenu(int nID);
	void SigSort();

private:




	void ShowSubmenuDialog();

	SubmenuDialog		*m_pSubmenuDlg;

	Ui::InfoService *ui;
};

#endif // INFOSERVICE_H
