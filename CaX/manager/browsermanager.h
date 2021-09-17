#ifndef BROWSERMANAGER_H
#define BROWSERMANAGER_H

#include "basemanager.h"

class BrowserManager : public BaseManager
{
	Q_OBJECT
public:
	explicit BrowserManager(QObject *parent = nullptr);
	~BrowserManager();

	void RequestRoot();
	void RequestFolder();
	void RequestCopy();
	void RequestMove();
	void RequestDelete();
	void RequestCreate();
	void RequestRename();
	void RequestPlay();

signals:

	void SigRespError(QString errMsg);
	void SigRespList(QList<CJsonNode> list);

private slots:

	void SlotRespInfo(QString json, int nCmdID);

private:

	enum reqBrowserIndex {
		BROWSER_ROOT,
		BROWSER_FOLDER,
		BROWSER_COPY,
		BROWSER_MOVE,
		BROWSER_DELETE,
		BROWSER_CREATE,
		BROWSER_RENAME,
		BROWSER_PLAY,
//		BROWSER_PLAYLIST,
//		BROWSER_INFO,
//		BROWSER_INFO_BOT,
//		BROWSER_SET_ART,
//		BROWSER_CONVERT,
//		BROWSER_REPLAYGAIN_SET,
//		BROWSER_REPLAYGAIN_CLEAR,
//		BROWSER_IMPORT,
//		BROWSER_UPNP_FOLDER,
//		BROWSER_UPNP_PLAY,
		BROWSER_MAX
	};

	void ParseRoot(CJsonNode node);


};

#endif // BROWSERMANAGER_H
