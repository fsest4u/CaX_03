#ifndef ICONSERVICE_H
#define ICONSERVICE_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class IconServiceDelegate;
class Loading;

namespace Ui {
class IconService;
}

class IconService : public QWidget
{
	Q_OBJECT

public:
	explicit IconService(QWidget *parent = nullptr);
	~IconService();

	enum {
		ICON_SERVICE_BROWSER = 0,
		ICON_SERVICE_ISERVICE,
		ICON_SERVICE_INPUT,
		ICON_SERVICE_FM_RADIO,
		ICON_SERVICE_DAB_RADIO,
		ICON_SERVICE_GROUP_PLAY,
		ICON_SERVICE_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int nService = ICON_SERVICE_BROWSER);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QMap<int, bool> GetSelectMap() const;
	void SetSelectMap(const QMap<int, bool> &SelectMap);

	QStandardItemModel		*GetModel();
	IconServiceDelegate		*GetDelegate();

signals:

	void SigReqCoverArt(int id, int index, int mode);

private slots:

	void SlotDoubleClickItem(const QModelIndex& index);

private:

	void Initialize();
	QString GetGroupPlayStatus(int type);
	QString GetActiveChannel(int type);
	int	GetCoverID(QString url);

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconServiceDelegate		*m_Delegate;

	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;
	QMap<int, bool>			m_SelectMap;

	Ui::IconService *ui;
};

#endif // ICONSERVICE_H
