#ifndef DEVICELISTWINDOW_H
#define DEVICELISTWINDOW_H

#include <QWidget>

#include "util/CJsonNode.h"

class ListDevice;

namespace Ui {
class DeviceListWindow;
}

class DeviceListWindow : public QWidget
{
	Q_OBJECT

public:
	explicit DeviceListWindow(QWidget *parent = nullptr);
	~DeviceListWindow();

	CJsonNode GetDeviceList() const;
	void SetDeviceList(const CJsonNode &list);

signals:

	void SigSelectDevice(QString mac);
	void SigSelectCancel(QString mac);

private slots:

	void SlotSelectDevice(QString mac);
	void SlotSelectCancel(QString mac);

private:

	void ConnectSigToSlot();

	ListDevice	*m_pListDevice;

	CJsonNode	m_NodeList;
	Ui::DeviceListWindow *ui;
};

#endif // DEVICELISTWINDOW_H
