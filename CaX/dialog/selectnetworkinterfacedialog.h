#ifndef SELECTNETWORKINTERFACEDIALOG_H
#define SELECTNETWORKINTERFACEDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QListWidgetItem>

namespace Ui {
class SelectNetworkInterfaceDialog;
}

class SelectNetworkInterfaceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectNetworkInterfaceDialog(QWidget *parent = nullptr);
	~SelectNetworkInterfaceDialog();

	void SetList(QMap<QString, QNetworkInterface> map);

	QString GetIP();
	QNetworkInterface GetInterface();

private slots:

	void SlotDoubleClicked(QListWidgetItem* item);

private:

	QString								m_IP;
	QNetworkInterface					m_Interface;

	QMap<QString, QNetworkInterface>	m_Map;

	Ui::SelectNetworkInterfaceDialog *ui;
};

#endif // SELECTNETWORKINTERFACEDIALOG_H
