#ifndef BROWSERMANAGER_H
#define BROWSERMANAGER_H

#include "basemanager.h"
#include "sqlmanager.h"

class BrowserManager : public BaseManager
{
	Q_OBJECT
public:
	explicit BrowserManager(QObject *parent = nullptr);
	~BrowserManager();

	void RequestRoot(bool dirOnly = false);
	void RequestFolder(QString strPath, bool dirOnly = false, bool imageOnly = false);
	void RequestInfoBot(QString strPath, int nIndex);
	void RequestTrackPlay(QString root, QStringList dirs, QStringList files, int where = PLAY_CLEAR);
	void RequestPlaylistPlay(QString path, QList<int> indexes);
	void RequestReplayGain(QString root, QStringList paths, QString gain, int eventID);
	void RequestConvertFormat(QString root, QStringList paths, QString format, int eventID);
	void RequestCreate(QString path);
	void RequestRename(QString src, QString dst);
	void RequestDelete(QString root, QStringList paths, int eventID);
	void RequestCopy(QString srcRoot, QStringList srcPaths, QString dstRoot, bool move, int eventID);
	void RequestSetArt(QString root, QStringList files, QString image, QString thumb, int eventID);
	void RequestInfoTag(QString path);
	void RequestSetTag(CJsonNode node, QString path);
	void RequestCategoryList(int nCategory);
	void RequestScanDB(QString path, int eventID);
	void RequestRemoveDB(QString path, int eventID);
	void RequestRandom();


	bool GetOptPlaySubDir() const;
	void SetOptPlaySubDir(bool OptPlaySubDir);

	bool GetOptOverwrite() const;
	void SetOptOverwrite(bool OptOverwrite);

signals:

	void SigRespError(QString errMsg);
	void SigRespList(QList<CJsonNode> list);

	void SigCoverArtUpdate(QString filename, int nIndex, int mode);
	void SigInfoBotUpdate(CJsonNode node, int nIndex);
	void SigInfoTagUpdate(CJsonNode node);
	void SigRespCategoryList(QList<CJsonNode> list);

	void SigListUpdate();	// refresh

private slots:

	void SlotRespInfo(QString json, int nCmdID, int nIndex);
	void SlotRespCoverArt(QString fileName, int nIndex, int mode);

private:

	enum {
		BROWSER_ROOT = 0,
		BROWSER_FOLDER,
		BROWSER_INFO_BOT,
		BROWSER_TRACK_PLAY,
		BROWSER_PLAYLIST_PLAY,
		BROWSER_REPLAYGAIN,
		BROWSER_CONVERT_FORMAT,
		BROWSER_CREATE,
		BROWSER_RENAME,
		BROWSER_DELETE,
		BROWSER_COPY,
		BROWSER_MOVE,
		BROWSER_SCAN_DB,
		BROWSER_REMOVE_DB,
		BROWSER_SET_ART,
		BROWSER_INFO_TAG,
		BROWSER_SET_TAG,
		BROWSER_RANDOM,
		BROWSER_CATEGORY_LIST,
//		BROWSER_IMPORT,
//		BROWSER_UPNP_FOLDER,
//		BROWSER_UPNP_PLAY,
		BROWSER_MAX
	};

	void ParseFolder(CJsonNode node);
	void ParseInfoBot(CJsonNode node, int nIndex);
	void ParseInfoTag(CJsonNode node);
	void ParseCategoryList(CJsonNode result);

	SQLManager	*m_pSql;

	QList<CJsonNode>	m_NodeList;

	bool	m_OptPlaySubDir;
	bool	m_OptOverwrite;


};

#endif // BROWSERMANAGER_H
