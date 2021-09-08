#ifndef MENUICON_H
#define MENUICON_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class MenuIcon;
}

class MenuIconDelegate;

class MenuIcon : public QWidget
{
	Q_OBJECT

public:
	explicit MenuIcon(QWidget *parent = nullptr);
	~MenuIcon();

	enum menuIndex {
		MENU_AUDIO_CD = 0,
		MENU_PLAYLIST,
		MENU_BROWSER,
		MENU_ISERVICE,
		MENU_INPUT,
		MENU_FM_RADIO,
		MENU_DAB_RADIO,
		MENU_GROUP_PLAY,
		MENU_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int menuType = MENU_AUDIO_CD);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	MenuIconDelegate		*GetDelegate();

signals:

private slots:

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	MenuIconDelegate		*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::MenuIcon *ui;
};

#endif // MENUICON_H
