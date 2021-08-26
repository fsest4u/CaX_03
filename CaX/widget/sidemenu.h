#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SideMenu;
}

class QListView;
class QStandardItemModel;
class SideMenuDelegate;

class SideMenu : public QWidget
{
	Q_OBJECT

public:
	explicit SideMenu(QWidget *parent = nullptr);
	~SideMenu();

	void ShowMenu();
	void HideMenu();

	void SetEnableAudioCD(bool bEnable);
	void SetEnableInput(bool bEnable);
	void SetEnableFMRadio(bool bEnable);
	void SetEnableDABRadio(bool bEnable);
	void SetEnableGroupPlay(bool bEnable);

signals:

	void SigSelectSideMenu(int menuIndex);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotSelectItem(QModelIndex index);

private:

	void Initialize();
	void SetMenu();
	void ClearMenu();


	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	SideMenuDelegate	*m_Delegate;

	bool m_bAudioCD;
	bool m_bInput;
	bool m_bFMRadio;
	bool m_bDABRadio;
	bool m_bGroupPlay;

	Ui::SideMenu *ui;
};

#endif // SIDEMENU_H
