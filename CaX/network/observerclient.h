#ifndef OBSERVERCLIENT_H
#define OBSERVERCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>


class ObserverClient :
	public QObject
{
	Q_OBJECT

public:

	explicit ObserverClient(QObject *parent = 0);
	~ObserverClient();

	void Connect(QString strAddr, QByteArray jsonData);
	void Disconnect();


signals:

	void SigConnect(QStringList jsonValueList);
	void SigDisconnect();

public slots:

	void SlotConnect();
	void SlotDisconnect();
	void SlotWrite(qint64 bytes);
	void SlotRead();


private:

	void ConnectSigToSlot();

	QString						m_StrAddr;
	int							m_nPort;

	int							m_nContentSize;

	QTcpSocket					*m_Socket;

};


#endif // OBSERVERCLIENT_H
