#ifndef IMANAGER_H
#define IMANAGER_H

#include "util/CJsonNode.h"

class TCPClient;

class IManager
{

public:

	virtual QString GetAddr() const = 0;
	virtual void SetAddr(const QString &Addr) = 0;

	virtual TCPClient *GetTcpClient() const = 0;
	virtual void SetTcpClient(TCPClient *pTcpClient) = 0;

	virtual void RequestCommand(CJsonNode node, int nCmdID, int nIndex) = 0;
	virtual void RequestCoverArt(QString fullpath, int index, int mode) = 0;
	virtual void RequestCoverArt(QString fullpath) = 0;

private:

};

#endif // IMANAGER_H
