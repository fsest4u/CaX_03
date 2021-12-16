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

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	int SetNodeList(const QList<CJsonNode> &NodeList, int nService);

	void SetNodeInfo(CJsonNode node);

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<QString, int> GetSelectMap() const;
	void SetSelectMap(const QMap<QString, int> &SelectMap);

	QMap<int, CJsonNode> GetSelectMapIService() const;
	void SetSelectMapIService(const QMap<int, CJsonNode> &SelectMap);

	void SetEditor(int index);

	QStandardItemModel	*GetModel();
	ListBrowserDelegate	*GetDelegate();

	void SetBackgroundTask(QThread *thread);

signals:

	void SigReqCoverArt(QString url, int nIndex);
	void SigReqInfoBot(QString path, int index);

private slots:

	void SlotReqCoverArt();
	void SlotFinishThread();
	void SlotDoubleClickItem(const QModelIndex& index);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListBrowserDelegate		*m_Delegate;

//	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<QString, int>		m_SelectMap;
	QMap<int, CJsonNode>	m_SelectMapIService;

	Ui::ListBrowser *ui;
};

#endif // LISTBROWSER_H
