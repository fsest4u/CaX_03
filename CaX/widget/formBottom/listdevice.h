#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class ListDeviceDelegate;
class Loading;

namespace Ui {
class ListDevice;
}

class ListDevice : public QWidget
{
	Q_OBJECT

public:
	explicit ListDevice(QWidget *parent = nullptr);
	~ListDevice();

	QListView::ViewMode GetViewMode() const;
	void SetViewMode(QListView::ViewMode mode);

	QStandardItemModel *GetModel() const;
	void SetModel(QStandardItemModel *Model);

	ListDeviceDelegate *GetDelegate() const;
	void SetDelegate(ListDeviceDelegate *Delegate);

	void ClearNodeList();
	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(const QList<CJsonNode> &NodeList);

private:

	void Initialize();

	QListView			*m_ListView;
	QStandardItemModel	*m_Model;
	ListDeviceDelegate	*m_Delegate;

	Loading					*m_pLoading;

	QList<CJsonNode>	m_NodeList;

	Ui::ListDevice *ui;
};

#endif // LISTDEVICE_H
