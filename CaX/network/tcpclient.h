#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>

class TCPClient : public QObject
{
	Q_OBJECT
public:
	explicit TCPClient(QObject *parent = nullptr);
	~TCPClient();

	void		RequestCommand(QByteArray jsonData, int nCmdID = -1, int nIndex = -1);

	void		RequestCoverArt(QString fullpath, int index, int mode);
	void		RequestCoverArt(QString fullpath);
	void		RequestSearchCoverArt(QString strUrl, int index);


	QString GetAddr() const;
	void SetAddr(const QString &Addr);


signals :

	void		SigRespInfo(QString jsonData, int nCmdID = -1, int nIndex = -1);
	void        SigRespCoverArt(QString fileName, int nIndex, int mode);
	void        SigRespCoverArt(QString fileName);
	void		SigRespSearchCoverArt(QByteArray data, int index);
//	void		SigRespSearchCoverArt(QString filename, int index);


public slots :


private :

	QNetworkAccessManager		*m_pManager;
	QNetworkReply               *m_pReply;
	QList<QNetworkReply*>		m_ListReply;

	QString						m_Addr;
	QString                     m_DirTemp;


};

#endif // TCPCLIENT_H
