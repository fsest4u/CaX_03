#include "selectnetworkinterfacedialog.h"
#include "ui_selectnetworkinterfacedialog.h"

#include "util/log.h"

SelectNetworkInterfaceDialog::SelectNetworkInterfaceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectNetworkInterfaceDialog)
{
	ui->setupUi(this);

	m_Map.clear();

	connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(SlotDoubleClicked(QListWidgetItem*)));
}

SelectNetworkInterfaceDialog::~SelectNetworkInterfaceDialog()
{
	delete ui;
}

void SelectNetworkInterfaceDialog::SetList(QMap<QString, QNetworkInterface> map)
{
	if (map.count() <= 0)
	{
		return;
	}

	m_Map = map;
	QMap<QString, QNetworkInterface>::iterator i;
	for (i = map.begin(); i!= map.end(); i++)
	{
		LogDebug("key [%s]", i.key().toUtf8().data());
		ui->listWidget->addItem(i.key());
	}

	ui->listWidget->setCurrentRow(0);
}

QString SelectNetworkInterfaceDialog::GetIP()
{
	QListWidgetItem *item = ui->listWidget->currentItem();
	m_IP = item->text().toUtf8();
	LogDebug("select ip [%s]", m_IP.toUtf8().data());

	return m_IP;
}

QNetworkInterface SelectNetworkInterfaceDialog::GetInterface()
{
	QListWidgetItem *item = ui->listWidget->currentItem();
	m_IP = item->text().toUtf8();
	m_Interface= m_Map.value(m_IP);
	LogDebug("select interface [%s] ip [%s]", m_Interface.hardwareAddress().toUtf8().data(), m_IP.toUtf8().data());

	return m_Interface;
}

void SelectNetworkInterfaceDialog::SlotDoubleClicked(QListWidgetItem *item)
{
//	m_IP = item->text().toUtf8();
//	m_Interface= m_Map.value(m_IP);

	done(QDialog::Accepted);
}
