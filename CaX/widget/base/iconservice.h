#ifndef ICONSERVICE_H
#define ICONSERVICE_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class IconService;
}

class IconServiceDelegate;

class IconService : public QWidget
{
	Q_OBJECT

public:
	explicit IconService(QWidget *parent = nullptr);
	~IconService();

	enum {
		ICON_SERVICE_AUDIO_CD = 0,
		ICON_SERVICE_PLAYLIST,
		ICON_SERVICE_BROWSER,
		ICON_SERVICE_ISERVICE,
		ICON_SERVICE_INPUT,
		ICON_SERVICE_FM_RADIO,
		ICON_SERVICE_DAB_RADIO,
		ICON_SERVICE_GROUP_PLAY,
		ICON_SERVICE_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList, int nService = ICON_SERVICE_AUDIO_CD);
	void ClearNodeList();

	QListView::ViewMode GetViewMode();
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel		*GetModel();
	IconServiceDelegate		*GetDelegate();

signals:

private slots:

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconServiceDelegate		*m_Delegate;

	QList<CJsonNode>	m_NodeList;

	Ui::IconService *ui;
};

#endif // ICONSERVICE_H
