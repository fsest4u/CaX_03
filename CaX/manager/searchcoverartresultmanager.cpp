#include "searchcoverartresultmanager.h"

SearchCoverArtResultManager::SearchCoverArtResultManager(QObject *parent)
{
	Q_UNUSED(parent)

	connect((QObject*)GetTcpClient(), SIGNAL(SigRespInfo(QString, int)), this, SLOT(SlotRespInfo(QString, int)));
//	connect((QObject*)GetTcpClient(), SIGNAL(SigRespSearchCoverArt(QByteArray, int)), this, SLOT(SlotRespSearchCoverArt(QByteArray, int)));
	connect((QObject*)GetTcpClient(), SIGNAL(SigRespSearchCoverArt(QString, int)), this, SLOT(SlotRespSearchCoverArt(QString, int)));
}

SearchCoverArtResultManager::~SearchCoverArtResultManager()
{

}

void SearchCoverArtResultManager::RequestCoverArtList(QString site,
													  QString keyword,
													  QString artist,
													  QString nextUrl)
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_CMD0, VAL_ETC);
	node.Add(KEY_CMD1, VAL_COVER_ART);
	node.Add(KEY_CMD2, VAL_SEARCH);
	node.Add(KEY_SITE, site);
	node.Add(KEY_SEARCH1, keyword);
	node.Add(KEY_SEARCH2, artist);
	if (!nextUrl.isEmpty())
	{
		node.Add(KEY_NEXT_URL, nextUrl);
	}

	RequestCommand(node, SEARCH_RESULT_COVER_ART_LIST);
}

void SearchCoverArtResultManager::SlotRespInfo(QString json, int nCmdID)
{
	if (json.isEmpty())
	{
		return;
	}

	CJsonNode node;
	if (!node.SetContent(json))
	{
		emit SigRespError(STR_INVALID_JSON);
		return;
	}

	LogDebug("cmdID [%d] node [%s]", nCmdID, node.ToTabedByteArray().data());

	QString message = node.GetString(VAL_MSG);
	bool success = node.GetBool(VAL_SUCCESS);
	if (!success)
	{
		if (message.isEmpty())
		{
			emit SigRespError(STR_UNKNOWN_ERROR);
			return;
		}

		emit SigRespError(message.left((MSG_LIMIT_COUNT)));
		return;
	}

	switch (nCmdID)
	{
	case SEARCH_RESULT_COVER_ART_LIST:
		ParseCoverArtList(node);
		return;
	case SEARCH_RESULT_MAX:
		emit SigRespError(STR_INVALID_ID);
		return;
	}

}

//void SearchCoverArtResultManager::SlotRespSearchCoverArt(QByteArray data, int index)
//{
//	emit SigRespSearchCoverArt(data, index);
//}

void SearchCoverArtResultManager::SlotRespSearchCoverArt(QString filename, int index)
{
	emit SigRespSearchCoverArt(filename, index);
}

void SearchCoverArtResultManager::ParseCoverArtList(CJsonNode node)
{
	emit SigRespCoverArtList(node);
}
