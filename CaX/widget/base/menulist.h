#ifndef MENULIST_H
#define MENULIST_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class MenuList;
}

class MenuListDelegate;

class MenuList : public QWidget
{
	Q_OBJECT

public:
	explicit MenuList(QWidget *parent = nullptr);
	~MenuList();

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

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel	*GetModel();
	MenuListDelegate	*GetDelegate();

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int menuType = MENU_AUDIO_CD);

signals:

	void SigReqArt(QString url, int nIndex);
	void SigReqInfoBot(QString path, int index);

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	MenuListDelegate	*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::MenuList *ui;
};

#endif // MENULIST_H
