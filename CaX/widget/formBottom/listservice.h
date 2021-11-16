#ifndef LISTSERVICE_H
#define LISTSERVICE_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class ListServiceDelegate;
class Loading;

namespace Ui {
class ListService;
}

class ListService : public QWidget
{
	Q_OBJECT

public:
	explicit ListService(QWidget *parent = nullptr);
	~ListService();

	enum {
//		LIST_SERVICE_AUDIO_CD = 0,
//		LIST_SERVICE_PLAYLIST,
//		LIST_SERVICE_BROWSER,
//		LIST_SERVICE_ISERVICE,
//		LIST_SERVICE_INPUT,
//		LIST_SERVICE_FM_RADIO,
//		LIST_SERVICE_DAB_RADIO,
//		LIST_SERVICE_GROUP_PLAY,
		LIST_SERVICE_SETUP,
		LIST_SERVICE_MAX
	};

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int nService = LIST_SERVICE_SETUP);

	void ClearSelectMap();
	void SetAllSelectMap();

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	QStandardItemModel	*GetModel();
	ListServiceDelegate	*GetDelegate();


signals:

	void SigReqArt(QString url, int nIndex);
//	void SigReqInfoBot(QString path, int index);

private slots:

	void SlotDoubleClickItem(const QModelIndex& index);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	ListServiceDelegate		*m_Delegate;

	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;

	Ui::ListService *ui;
};

#endif // LISTSERVICE_H
