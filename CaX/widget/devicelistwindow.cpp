#include "devicelistwindow.h"
#include "ui_devicelistwindow.h"

#include "formBottom/listdevice.h"
#include "formBottom/listdevicedelegate.h"

#include "util/loading.h"
#include "util/log.h"

DeviceListWindow::DeviceListWindow(QWidget *parent) :
	QWidget(parent),
	m_pListDevice(new ListDevice(this)),
	ui(new Ui::DeviceListWindow)
{
	ui->setupUi(this);

	ConnectSigToSlot();

	ui->gridLayout->addWidget(m_pListDevice);

	m_pLoading = new Loading(ui->widget);
	m_pLoading->Start();

}

DeviceListWindow::~DeviceListWindow()
{
	m_pLoading->Stop();

	if (m_pListDevice)
	{
		delete m_pListDevice;
		m_pListDevice = nullptr;
	}

	if (m_pLoading)
	{
		delete m_pLoading;
		m_pLoading = nullptr;
	}

	delete ui;
}

CJsonNode DeviceListWindow::GetDeviceList() const
{
	return m_NodeList;
}

void DeviceListWindow::SetDeviceList(const CJsonNode &list)
{
	m_NodeList = list;

	QList<CJsonNode> nodeList;
	for (int i = 0; i < m_NodeList.ArraySize(); i++)
	{
		nodeList.append(m_NodeList.GetArrayAt(i));
	}
	m_pListDevice->SetNodeList(nodeList);
}

QString DeviceListWindow::GetTitle() const
{
	return ui->labelTitle->text();
}

void DeviceListWindow::SetTitle(const QString &Title)
{
	ui->labelTitle->setText(Title);
}

void DeviceListWindow::SlotSelectDevice(QString mac)
{
	emit SigSelectDevice(mac);
}

void DeviceListWindow::SlotSelectCancel(QString mac)
{
	emit SigSelectCancel(mac);
}

void DeviceListWindow::ConnectSigToSlot()
{
	connect(m_pListDevice->GetDelegate(), SIGNAL(SigSelectDevice(QString)), this, SLOT(SlotSelectDevice(QString)));
	connect(m_pListDevice->GetDelegate(), SIGNAL(SigSelectCancel(QString)), this, SLOT(SlotSelectCancel(QString)));

}
