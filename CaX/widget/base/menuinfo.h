#ifndef MENUINFO_H
#define MENUINFO_H

#include <QWidget>

namespace Ui {
class MenuInfo;
}

class SubmenuDialog;

class MenuInfo : public QWidget
{
	Q_OBJECT

public:
	explicit MenuInfo(QWidget *parent = nullptr);
	~MenuInfo();

	enum iServiceSubmenuType {
		ISERVICE_NONE,
		ISERVICE_MAX
	};

	enum fmSubmenuType {
		FM_SEARCH_ALL_DELETE = Qt::UserRole + 0,
		FM_SEARCH_ALL,
		FM_ADD,
		FM_DELETE,
		FM_RESERVE_LIST,
		FM_MAX
	};

	enum dabSubmenuType {
		DAB_SEARCH_ALL_DELETE = Qt::UserRole + 100,
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

	Ui::MenuInfo *ui;
};

#endif // MENUINFO_H
