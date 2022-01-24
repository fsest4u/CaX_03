#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include <QObject>

#include "imanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

class BaseManager : public QObject, public IManager
{
	Q_OBJECT
public:
	explicit BaseManager(QObject *parent = nullptr);
	~BaseManager();

	QString GetAddr() const;
	void SetAddr(const QString &Addr);

	TCPClient *GetTcpClient() const;
	void SetTcpClient(TCPClient *pTcpClient);

	void RequestCommand(CJsonNode node, int nCmdID = -1, int nIndex = -1);
	void RequestCoverArt(QString fullpath, int index, int mode);
	void RequestCoverArt(QString fullpath);
	void RequestSearchCoverArt(QString strUrl, int index);

private:

	TCPClient	*m_pTcpClient;

};

#endif // BASEMANAGER_H
