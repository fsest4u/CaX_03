#ifndef SEARCHCOVERARTRESULTMANAGER_H
#define SEARCHCOVERARTRESULTMANAGER_H

#include <QObject>

#include "basemanager.h"

class SearchCoverArtResultManager : public BaseManager
{
	Q_OBJECT
public:
	SearchCoverArtResultManager(QObject *parent = nullptr);
	~SearchCoverArtResultManager();

	enum {
		SEARCH_RESULT_COVER_ART_LIST,
		SEARCH_RESULT_MAX
	};

	void RequestCoverArtList(QString site, QString keyword, QString artist, QString nextUrl = "");

signals:

	void SigRespError(QString errMsg);
	void SigRespCoverArtList(CJsonNode node);
	void SigRespSearchCoverArt(QString filename, int index);


private slots:

	void SlotRespInfo(QString json, int nCmdID);
	void SlotRespSearchCoverArt(QString filename, int index);

private:

	void ParseCoverArtList(CJsonNode node);

};

#endif // SEARCHCOVERARTRESULTMANAGER_H
