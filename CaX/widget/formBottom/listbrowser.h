#ifndef LISTBROWSER_H
#define LISTBROWSER_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class ListBrowserDelegate;
class Loading;

namespace Ui {
class ListBrowser;
}

class ListBrowser : public QWidget
{
	Q_OBJECT

public:
	explicit ListBrowser(QWidget *parent = nullptr);
	~ListBrowser();

//	enum {
//		LIST_BROWSER_BROWSER = 0,
//		LIST_BROWSER_ISERVICE,
//		LIST_BROWSER_MAX
//	};

	QList<CJsonNode> GetNodeList() const;
	int SetNodeList(const QList<CJsonNode> list, int service);
	void ClearNodeList();

	void SetNodeInfo(CJsonNode node);

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<QString, int> GetSelectMap() const;
	void SetSelectMap(const QMap<QString, int> &SelectMap);

	QMap<int, CJsonNode> GetSelectMapIService() const;
	void SetSelectMapIService(const QMap<int, CJsonNode> &SelectMap);

	void SetEditor(int index);

	QListView			*GetListView();
	QStandardItemModel	*GetModel();
	ListBrowserDelegate	*GetDelegate();

signals:

	void SigReqCoverArt(QString url, int nIndex);
	void SigReqInfoBot(QString path, int index);
	void SigAppendList();

private slots:

	void SlotScrollValueChanged(int value);
	void SlotSelectCheck(const QModelIndex &modelIndex);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListBrowserDelegate		*m_Delegate;
	QScrollBar				*m_ScrollBar;

//	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<QString, int>		m_SelectMap;
	QMap<int, CJsonNode>	m_SelectMapIService;

	Ui::ListBrowser *ui;
};

#endif // LISTBROWSER_H
