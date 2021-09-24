#ifndef LISTSERVICE_H
#define LISTSERVICE_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class ListService;
}

class ListServiceDelegate;

class ListService : public QWidget
{
	Q_OBJECT

public:
	explicit ListService(QWidget *parent = nullptr);
	~ListService();

	enum {
		LIST_SERVICE_AUDIO_CD = 0,
		LIST_SERVICE_PLAYLIST,
		LIST_SERVICE_BROWSER,
		LIST_SERVICE_ISERVICE,
		LIST_SERVICE_INPUT,
		LIST_SERVICE_FM_RADIO,
		LIST_SERVICE_DAB_RADIO,
		LIST_SERVICE_GROUP_PLAY,
		LIST_SERVICE_MAX
	};

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel	*GetModel();
	ListServiceDelegate	*GetDelegate();

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int nService = LIST_SERVICE_AUDIO_CD);

signals:

	void SigReqArt(QString url, int nIndex);
	void SigReqInfoBot(QString path, int index);

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	ListServiceDelegate	*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::ListService *ui;
};

#endif // LISTSERVICE_H
