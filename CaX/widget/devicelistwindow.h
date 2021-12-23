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

//	enum {
//		MODE_DEVICE_SELECT = Qt::UserRole + 0,
//		MODE_DEVICE_POWER_ON
//	};

	CJsonNode GetDeviceList() const;
	void SetDeviceList(const CJsonNode &list);

	QString GetTitle() const;
	void SetTitle(const QString &Title);

signals:

	void SigSelectDevice(QString mac, QString addr, QString val, QString dev);
	void SigSelectCancel(QString mac, QString addr, QString val, QString dev);

private slots:

	void SlotSelectDevice(QString mac, QString addr, QString val, QString dev);
	void SlotSelectCancel(QString mac, QString addr, QString val, QString dev);

private:

	void ConnectSigToSlot();

	ListDevice				*m_pListDevice;
	CJsonNode				m_NodeList;

	Ui::DeviceListWindow *ui;
};

#endif // DEVICELISTWINDOW_H
