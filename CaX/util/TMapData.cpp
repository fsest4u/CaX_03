//#include "HwDef.h"

#include "StringLib.h"

#include "TMapData.h"

bool MapStrSet(TMapData &Md, int iStrKey, QString strValue, bool &blChange)
{
	if( MapStrVal(Md, iStrKey) == strValue )	return false;

	MapStrAdd(Md, iStrKey, strValue);
	blChange = true;

	return true;
}

bool MapIntSet(TMapData &Md, int iIntKey, int iValue, bool &blChange)
{
	if( MapIntVal(Md, iIntKey) == iValue )	return false;

	MapIntAdd(Md, iIntKey, (int64_t)iValue);
	blChange = true;

	return true;
}

bool MapBoolSet(TMapData &Md, int iChrKey, bool blValue, bool &blChange)
{
	if( ( MapChrVal(Md, iChrKey) == cVal_True ) == blValue )	return false;

	MapChrAdd(Md, iChrKey, blValue? cVal_True : cVal_False);
	blChange = true;

	return true;
}

void MapDataMergy(TMapData &MdTo, TMapData &MdFrom)
{
	QMap<int, int64_t>::iterator itInt;
	for(itInt = MdFrom.mapInt.begin(); itInt != MdFrom.mapInt.end(); itInt++)	MapIntAdd(MdTo, itInt.key(), itInt.value());

	QMap<int, QString>::iterator itStr;
	for(itStr = MdFrom.mapStr.begin(); itStr != MdFrom.mapStr.end(); itStr++)	MapStrAdd(MdTo, itStr.key(), itStr.value());

	QMap<int, char>::iterator itCh;
	for(itCh = MdFrom.mapChar.begin(); itCh != MdFrom.mapChar.end(); itCh++)	MapChrAdd(MdTo, itCh.key(), itCh.value());
}

bool SetMapDataIfEmpty(int iStrKey, QString strValue, TMapData &MapData)
{
	Trimmed(strValue);
	if( strValue.isEmpty() || !MapStrVal(MapData, iStrKey).isEmpty() )		return false;

	MapStrAdd(MapData, iStrKey, strValue);

	return true;
}

bool SetMapDataIfNotEmpty(int iStrKey, QString strValue, TMapData &MapData)
{
	Trimmed(strValue);
	if( strValue.isEmpty() )						return false;
	if( !MapStrVal(MapData, iStrKey).isEmpty() )	return false;

	MapStrAdd(MapData, iStrKey, strValue);

	return true;
}

bool ReplaceMapDataList(TMapDataList &lsMapData, int index, int iStrKey, QString str)
{
	if( index < 0 || index >= lsMapData.size() )	return false;

	TMapData MapData = lsMapData.at(index);
	MapStrAdd(MapData, iStrKey, str);
	lsMapData.replace(index, MapData);

	return true;
}

int FindMapDataListFromStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey)			// Get MapData from StrKey
{
	QString strValue = MapStrVal(MdData, iStrKey);
	if( strValue.isEmpty() )
	{
		LogCritical("strValue is Empty !\n");
		return -1;
	}

	for(int i = 0; i < lsMapData.size(); i++)
	{
		if( MapStrVal(lsMapData.at(i), iStrKey) == strValue )
		{
			MdData = lsMapData.at(i);
			return i;
		}
	}

	return -1;
}

int FindMapDataList(TMapDataList &lsMapData, TMapData &MdData, int iKey, int64_t llValue)		// Get MapData from iKey & Value
{
	for(int i = 0; i < lsMapData.size(); i++)
	{
		if( MapIntVal(lsMapData.at(i), iKey) == llValue )
		{
			MdData = lsMapData.at(i);
			return i;
		}
	}

	return -1;
}

int FindMapDataList(TMapDataList &lsMapData, TMapData &MdData, int iStrKey, QString strValue)		// Get MapData from StrKey & Value
{
	for(int i = 0; i < lsMapData.size(); i++)
	{
		if( MapStrVal(lsMapData.at(i), iStrKey) == strValue )
		{
			MdData = lsMapData.at(i);
			return i;
		}
	}

	return -1;
}

bool AppendMapDataListCheckStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey)		// StrKey is not exist then append
{
	QString strValue = MapStrVal(MdData, iStrKey);
	if( strValue.isEmpty() )
	{
		LogCritical("strValue is Empty !\n");
		return false;
	}

	for(int i = 0; i < lsMapData.size(); i++)
	{
		if( MapStrVal(lsMapData.at(i), iStrKey) == strValue )	return false;
	}

	lsMapData.append(MdData);

	return true;
}

bool RemoveMapDataListFromStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey)		// StrKey is exist then Remove & Get Removed MapData
{
	QString strValue = MapStrVal(MdData, iStrKey);
	if( strValue.isEmpty() )
	{
		LogCritical("strValue is Empty !\n");
		return false;
	}

	for(int i = 0; i < lsMapData.size(); i++)
	{
		if( MapStrVal(lsMapData.at(i), iStrKey) == strValue )
		{
			MdData = lsMapData.at(i);
			lsMapData.removeAt(i);
			return true;
		}
	}

	return false;
}

void ClearMapData(TMapData *lpMapData)
{
	lpMapData->mapInt.clear();
	lpMapData->mapStr.clear();
	lpMapData->mapChar.clear();
}

#ifndef NO_LOGGING
void InitLogMapData();

bool g_blInitLogMapData = false;

QMap<int, QString> g_mapLogIntKey;
QMap<int, QString> g_mapLogStrKey;
QMap<int, QString> g_mapLogCharKey;

extern int g_iLogLevel;

void _LogMapData(TMapData MdInfo, QString strName)
{
//	if( g_iLogLevel > WS_LOGLEVEL_WARNING )		return;

	LogDebug("<<<<<<<<<<<<<<<<<<<< MapData : %s\n", ToAscii(strName).data());
	if( !g_blInitLogMapData )	InitLogMapData();

	QMap<int, QString>::iterator itStr;
	for(itStr = MdInfo.mapStr.begin(); itStr != MdInfo.mapStr.end(); itStr++)
	{
		LogDebug("\tStr %2d : %-22s = \"%s\"\n", itStr.key(), ToAscii(g_mapLogStrKey.value(itStr.key())).data(), ToAscii(QString(itStr.value())).data());
	}
	QMap<int, int64_t>::iterator itInt;
	for(itInt = MdInfo.mapInt.begin(); itInt != MdInfo.mapInt.end(); itInt++)
	{
		LogDebug("\tInt %2d : %-22s = %8lld : 0x%llX\n", itInt.key(), ToAscii(g_mapLogIntKey.value(itInt.key())).data(), itInt.value(), itInt.value());
	}
	QMap<int, char>::iterator itCh;
	for(itCh = MdInfo.mapChar.begin(); itCh != MdInfo.mapChar.end(); itCh++)
	{
		LogDebug("\tChr %2d : %-22s = %d\n", itCh.key(), ToAscii(g_mapLogCharKey.value(itCh.key())).data(), (int)itCh.value());
	}
	LogDebug(">>>>>>>>>>>>>>>>>>>> MapData : %s\n", ToAscii(strName).data());
}

void InitLogMapData()
{
	// Int Keys <<
	g_mapLogIntKey.insert(iKey_Type,			"Type");			// Tst_???
	g_mapLogIntKey.insert(iKey_SubType,			"Sub Type");		// Tsst_???
	g_mapLogIntKey.insert(iKey_ID,				"ID");
	g_mapLogIntKey.insert(iKey_SubID,			"Sub ID");

	g_mapLogIntKey.insert(iKey_ArtistID,		"Artist ID");
	g_mapLogIntKey.insert(iKey_AlbumArtistID,	"Album Artist ID");
	g_mapLogIntKey.insert(iKey_GenreID,			"Genre ID");
	g_mapLogIntKey.insert(iKey_AlbumID,			"Album ID");
	g_mapLogIntKey.insert(iKey_ComposerID,		"Composer ID");
	g_mapLogIntKey.insert(iKey_MoodID,			"Mood ID");
	g_mapLogIntKey.insert(iKey_FolderID,		"Folder ID");
	g_mapLogIntKey.insert(iKey_ScanPathID,		"Scan Path ID");
	g_mapLogIntKey.insert(iKey_FilePathID,		"File Path ID");
	g_mapLogIntKey.insert(iKey_FsUuid,			"FsUuid");

	g_mapLogIntKey.insert(iKey_Track,			"Track");
	g_mapLogIntKey.insert(iKey_Year,			"Year");
	g_mapLogIntKey.insert(iKey_CdYear,			"Cd Year");
	g_mapLogIntKey.insert(iKey_CdTotal,			"Cd Total");
	g_mapLogIntKey.insert(iKey_CdNumber,		"Cd Number");
	g_mapLogIntKey.insert(iKey_NewCdNumber,		"New Cd Number");

	g_mapLogIntKey.insert(iKey_Duration,		"Duration");
	g_mapLogIntKey.insert(iKey_DurationMs,		"Duration Ms");
	g_mapLogIntKey.insert(iKey_SecMs,			"Sec Ms");
	g_mapLogIntKey.insert(iKey_BitRate,			"Bit Rate");
	g_mapLogIntKey.insert(iKey_SampleRate,		"Sample Rate");
	g_mapLogIntKey.insert(iKey_Channel,			"Channel");

	g_mapLogIntKey.insert(iKey_SampleCnt,		"Sample Cnt");
	g_mapLogIntKey.insert(iKey_BitsPerSample,	"Bits Per Sample");
	g_mapLogIntKey.insert(iKey_BytesPerSample,	"Bytes Per Sample");
	g_mapLogIntKey.insert(iKey_CodecId,			"Codec ID");

	g_mapLogIntKey.insert(iKey_Int,				"Int");				// int value
	g_mapLogIntKey.insert(iKey_Seq,				"Seq");				// Sequence
	g_mapLogIntKey.insert(iKey_Speed,			"Speed");
	g_mapLogIntKey.insert(iKey_Port,			"Port");
	g_mapLogIntKey.insert(iKey_Major,			"Major");
	g_mapLogIntKey.insert(iKey_Minor,			"Minor");
	g_mapLogIntKey.insert(iKey_StatusCode,		"Status Code");
	g_mapLogIntKey.insert(iKey_Format,			"Format");
	g_mapLogIntKey.insert(iKey_Sec,				"Sec");

	g_mapLogIntKey.insert(iKey_Pointer,				"Pointer");					// Poiner variable
	g_mapLogIntKey.insert(iKey_PointerThis,			"Pointer This");			// Poiner variable
	g_mapLogIntKey.insert(iKey_PointerStringList,	"Pointer StringList");		// Poiner variable
	g_mapLogIntKey.insert(iKey_PointerMapDataList,	"Pointer MapDataList");		// Poiner variable
	g_mapLogIntKey.insert(iKey_PointerIntList,		"Pointer IntList");			// Poiner variable

	g_mapLogIntKey.insert(iKey_FirstSector,		"First Secotr");
	g_mapLogIntKey.insert(iKey_LastSector,		"Last Sector");

	g_mapLogIntKey.insert(iKey_Bus,				"Bus");				// QThreadUdev <<
	g_mapLogIntKey.insert(iKey_IfType,			"If Type");			// enumNetworkConnection : NC_NONE, NC_WIRED, NC_WIRELESS
	g_mapLogIntKey.insert(iKey_CdMedia,			"Cd Media");		// QThreadUdev >>	// iCdMedia_None, iCdMedia_Audio, iCdMedia_Data

	g_mapLogIntKey.insert(iKey_Menu,			"Menu");			// QwPlayMenu <<
	g_mapLogIntKey.insert(iKey_MenuDepth,		"Menu Depth");		// 1, 2, 3, ...
	g_mapLogIntKey.insert(iKey_Select,			"Select");			// Curr. selected index
	g_mapLogIntKey.insert(iKey_Sort,			"Sort");
	g_mapLogIntKey.insert(iKey_Table,			"Table");			// iTable_???
	g_mapLogIntKey.insert(iKey_Error,			"Error");
	g_mapLogIntKey.insert(iKey_View,			"View");
	g_mapLogIntKey.insert(iKey_ViewKey,			"View Key");		// QwPlayMenu >>

	g_mapLogIntKey.insert(iKey_Wl_Channel,				"Wl Channel");				// Wireless Key <<
	g_mapLogIntKey.insert(iKey_Wl_Sensitivity,			"Wl Sensitivity");
	g_mapLogIntKey.insert(iKey_Wl_Encryption,			"Wl Encryption");
	g_mapLogIntKey.insert(iKey_Wl_SupportedEncryption,	"Wl SupportedEncryption");
	g_mapLogIntKey.insert(iKey_Wl_Mode,					"Wl Mode");
	g_mapLogIntKey.insert(iKey_Wl_Setup,				"Wl Setup");
	g_mapLogIntKey.insert(iKey_Wl_Security,				"Wl Security");
	g_mapLogIntKey.insert(iKey_Wl_SupportedSecurity,	"Wl SupportedSecurity");	// Wireless Key >>

	g_mapLogIntKey.insert(iKey_FrameSize,		"Frame Size");
	g_mapLogIntKey.insert(iKey_SampleSize,		"Sample Size");
	g_mapLogIntKey.insert(iKey_CtlPort,			"Control Port");

	g_mapLogIntKey.insert(iKey_Freq,			"Frequency");					// for Radio
	g_mapLogIntKey.insert(iKey_SvcID,			"Service ID");					// for DAB
	g_mapLogIntKey.insert(iKey_CompID,			"Component ID");				// for DAB
	g_mapLogIntKey.insert(iKey_SvcCompType,		"Service Component Type");		// for DAB
	g_mapLogIntKey.insert(iKey_EnsembleID,		"Ensemble ID");					// for DAB

	g_mapLogIntKey.insert(iKey_PcmMask,			"PCM Sound Format Mask");
	g_mapLogIntKey.insert(iKey_SampleRateMask,	"SampleRate Mask");
	g_mapLogIntKey.insert(iKey_VendorID,		"Vendor ID");
	g_mapLogIntKey.insert(iKey_ModelID,			"Model ID");

	g_mapLogIntKey.insert(iKey_TrID,			"Tr ID");
	g_mapLogIntKey.insert(iKey_AppDoType,		"App Do Type");
	g_mapLogIntKey.insert(iKey_PageIndex,		"Page Index");
	g_mapLogIntKey.insert(iKey_PageSize,		"Page Size");

	g_mapLogIntKey.insert(iKey_Load_Priority,	"Load Priority");
	g_mapLogIntKey.insert(iKey_RaidMode,		"Raid Mode");

	g_mapLogIntKey.insert(iKey_TimeStemp,		"Time Stemp");
	g_mapLogIntKey.insert(iKey_Index,			"Index");
	g_mapLogIntKey.insert(iKey_Count,			"Count");
	g_mapLogIntKey.insert(iKey_Volume,			"Volume");

	g_mapLogIntKey.insert(iKey_ReplayGain,		"Replay Gain");
	g_mapLogIntKey.insert(iKey_ReplayPeak,		"Replay Peak");

	g_mapLogIntKey.insert(iKey_New,				"New");
	g_mapLogIntKey.insert(iKey_Curr,			"Curr");
	g_mapLogIntKey.insert(iKey_Src,				"Src");
	g_mapLogIntKey.insert(iKey_Dst,				"Dst");
	g_mapLogIntKey.insert(iKey_UrlIndex,		"Url Index");
	g_mapLogIntKey.insert(iKey_EventID,			"Event ID");
	g_mapLogIntKey.insert(iKey_FileManager,		"File Manager");

	g_mapLogIntKey.insert(iKey_PlaylistType,	"Youtube Playlist Type");
	g_mapLogIntKey.insert(iKey_Size,			"Size");
	g_mapLogIntKey.insert(iKey_Retry,			"Retry");
	g_mapLogIntKey.insert(iKey_Flag,			"Flag");

	g_mapLogIntKey.insert(iKey_PointerMdList,		"Pointer Md List");
	g_mapLogIntKey.insert(iKey_PointerMdImages,		"Pointer Md Images");
	g_mapLogIntKey.insert(iKey_PointerMdActions,	"Pointer Md Actions");

	g_mapLogIntKey.insert(iKey_LpLsInt,			"Int List Pointer");
	g_mapLogIntKey.insert(iKey_LpLsStr,			"Str List Pointer");
	g_mapLogIntKey.insert(iKey_LpLsStrStr,		"Str Str List Pointer");
	// Int Keys >>

	// Str Keys <<
	g_mapLogStrKey.insert(strKey_Type,			"Type");
	g_mapLogStrKey.insert(strKey_Url,			"Url");
	g_mapLogStrKey.insert(strKey_OrgUrl,		"Orgin Url");
	g_mapLogStrKey.insert(strKey_NextUrl,		"Next Url");
	g_mapLogStrKey.insert(strKey_FilePath,		"File Path");
	g_mapLogStrKey.insert(strKey_Disp,			"Disp");
	g_mapLogStrKey.insert(strKey_ID,			"ID");
	g_mapLogStrKey.insert(strKey_String,		"String");
	g_mapLogStrKey.insert(strKey_Text,			"Text");

	g_mapLogStrKey.insert(strKey_Name,			"Name");
	g_mapLogStrKey.insert(strKey_Title,			"Title");
	g_mapLogStrKey.insert(strKey_TitleTop,		"Title Top");
	g_mapLogStrKey.insert(strKey_StreamTitle,	"Stream Title");

	g_mapLogStrKey.insert(strKey_Artist,		"Artist");
	g_mapLogStrKey.insert(strKey_AlbumArtist,	"Album Artist");
	g_mapLogStrKey.insert(strKey_Genre,			"Genre");
	g_mapLogStrKey.insert(strKey_Album,			"Album");
	g_mapLogStrKey.insert(strKey_Composer,		"Composer");
	g_mapLogStrKey.insert(strKey_Mood,			"Mood");
	g_mapLogStrKey.insert(strKey_Folder,		"Folder");

	g_mapLogStrKey.insert(strKey_Track,			"Track");
	g_mapLogStrKey.insert(strKey_Year,			"Year");
	g_mapLogStrKey.insert(strKey_Tempo,			"Tempo");
	g_mapLogStrKey.insert(strKey_Format,		"Format");
	g_mapLogStrKey.insert(strKey_CdToc,			"CdToc");

	g_mapLogStrKey.insert(strKey_CoverArtPath,	"Cover Art Path");
	g_mapLogStrKey.insert(strKey_AlbumArtUrl,	"Album Art Url");
	g_mapLogStrKey.insert(strKey_LogoUrl,		"Logo Url");
	g_mapLogStrKey.insert(strKey_LogoType,		"Logo Type");

	g_mapLogStrKey.insert(strKey_Ext,			"Ext");
	g_mapLogStrKey.insert(strKey_Codec,			"Codec");
	g_mapLogStrKey.insert(strKey_Search,		"Search");
	g_mapLogStrKey.insert(strKey_TableName,		"Table Name");
	g_mapLogStrKey.insert(strKey_Uuid,			"UUID");
	g_mapLogStrKey.insert(strKey_Host,			"Host");
	g_mapLogStrKey.insert(strKey_Path,			"Path");
	g_mapLogStrKey.insert(strKey_FileName,		"File Name");
	g_mapLogStrKey.insert(strKey_Comment,		"Comment");
	g_mapLogStrKey.insert(strKey_Nickname,		"Nick Name");
	g_mapLogStrKey.insert(strKey_UncPath,		"Unc Path");
	g_mapLogStrKey.insert(strKey_RelPath,		"Rel Path");			// Path from MntPath
	g_mapLogStrKey.insert(strKey_MacAddr,		"Mac Addr");
	g_mapLogStrKey.insert(strKey_IpAddr,		"IP Addr");
	g_mapLogStrKey.insert(strKey_NetMask,		"Net Mask");
	g_mapLogStrKey.insert(strKey_Gateway,		"Gateway");
	g_mapLogStrKey.insert(strKey_DnsAddr,		"DNS Addr");

	g_mapLogStrKey.insert(strKey_Prog,			"Prog");			// QThreadUdev <<
	g_mapLogStrKey.insert(strKey_Option,		"Option");
	g_mapLogStrKey.insert(strKey_DevPath,		"Dev Path");
	g_mapLogStrKey.insert(strKey_DevName,		"Dev Name");
	g_mapLogStrKey.insert(strKey_FsType,		"FS Type");
	g_mapLogStrKey.insert(strKey_MntName,		"Mount Name");
	g_mapLogStrKey.insert(strKey_MntPath,		"Mount Path");
	g_mapLogStrKey.insert(strKey_Action,		"Action");
	g_mapLogStrKey.insert(strKey_If,			"Interface");
	g_mapLogStrKey.insert(strKey_FsUuid,		"FS UUID");			// QThreadUdev >>

	g_mapLogStrKey.insert(strKey_Wl_AP,			"Wl AP");			// Wireless Key
	g_mapLogStrKey.insert(strKey_Wl_WEP,		"Wl WEP");			// Wireless Key

	g_mapLogStrKey.insert(strKey_Schema,		"Schema");
	g_mapLogStrKey.insert(strKey_Version,		"Version");
	g_mapLogStrKey.insert(strKey_Method,		"Method");
	g_mapLogStrKey.insert(strKey_ReasonPhrase,	"Reason Phrase");
	g_mapLogStrKey.insert(strKey_DacpId,		"DACP-ID");
	g_mapLogStrKey.insert(strKey_ActiveRemote,	"Active-Remote");
	g_mapLogStrKey.insert(strKey_Domain,		"Domain");

	g_mapLogStrKey.insert(strKey_Res,				"Response");
	g_mapLogStrKey.insert(strKey_Workgroup,			"Workgroup");
	g_mapLogStrKey.insert(strKey_Server,			"Server");
	g_mapLogStrKey.insert(strKey_User,				"User");
	g_mapLogStrKey.insert(strKey_Password,			"Password");
	g_mapLogStrKey.insert(strKey_Share,				"Share");
	g_mapLogStrKey.insert(strKey_OldName,			"Old Name");
	g_mapLogStrKey.insert(strKey_Value,				"Value");
	g_mapLogStrKey.insert(strKey_ArtistImageUrl,	"Artist Image Url");

	g_mapLogStrKey.insert(strKey_Id_Path,		"ID Path");
	g_mapLogStrKey.insert(strKey_Category,		"Category");
	g_mapLogStrKey.insert(strKey_Site,			"Site");
	g_mapLogStrKey.insert(strKey_InPath,		"In Path");
	g_mapLogStrKey.insert(strKey_OutPath,		"Out Path");
	g_mapLogStrKey.insert(strKey_State,			"State");
	g_mapLogStrKey.insert(strKey_CardID,		"CardID");
	g_mapLogStrKey.insert(strKey_AudioFormat,	"Audio Format");

	g_mapLogStrKey.insert(strKey_AFavoriteUrl,		"AFavorite Proc Url");
	g_mapLogStrKey.insert(strKey_AEventStartUrl,	"AEvent Start Url");
	g_mapLogStrKey.insert(strKey_AEventStopUrl,		"AEvent Stop Url");
	g_mapLogStrKey.insert(strKey_AEventErrorUrl,	"AEvent Error Url");
	g_mapLogStrKey.insert(strKey_AEventPauseUrl,	"AEvent Pause Url");

#if USE_OLD_APPSERVER
	//g_mapLogStrKey.insert(strKey_ASkipUrl,			"Airable Skip Url");
#endif	// USE_OLD_APPSERVER

	//g_mapLogStrKey.insert(strKey_ASkipStatusUrl,	"Airable Skip Status Url");

	g_mapLogStrKey.insert(strKey_AEventTitle,			"Airable Event Title");
	g_mapLogStrKey.insert(stkKey_AEventLabel,			"Airable Event Label");
	g_mapLogStrKey.insert(strKey_AEventBtnYes,			"Airable Event Yes");
	g_mapLogStrKey.insert(strKey_AEventBtnCancel,		"Airable Event Cancel");
	g_mapLogStrKey.insert(strKey_AEventBtnYesUrl,		"Airable Event Yes Url");
	g_mapLogStrKey.insert(strKey_AEventBtnCancelUrl,	"Airable Event Cancel Url");

	g_mapLogStrKey.insert(strKey_ARatePositiveUrl,		"Airable Positive Url");
	g_mapLogStrKey.insert(strKey_ARateNeutralUrl,		"Airable Neutral Url");
	g_mapLogStrKey.insert(strKey_ARateNegativeUrl,		"Airable Negative Url");

	g_mapLogStrKey.insert(strKey_RaatOneLine,				"OneLine");			// Raat
	g_mapLogStrKey.insert(strKey_RaatTwoLineTitle,			"TwoLine0Title");
	g_mapLogStrKey.insert(strKey_RaatTwoLineSubtitle,		"TwoLine1Title");
	g_mapLogStrKey.insert(strKey_RaatThreeLineTitle,		"ThreeLine0Title");
	g_mapLogStrKey.insert(strKey_RaatThreeLineSubTitle,		"ThreeLine1Title");
	g_mapLogStrKey.insert(strKey_RaatThreeLineSubSubTitle,	"ThreeLine2Title");

	g_mapLogStrKey.insert(strKey_Param1,		"Param 1");

	g_mapLogStrKey.insert(strKey_Gn_TUI,						"TUI");			// Gracenote
	g_mapLogStrKey.insert(strKey_Gn_TUI_TAG,					"TUI_TAG");
	g_mapLogStrKey.insert(strKey_Gn_Album_Title_Official,		"Album_Title_Official");
	g_mapLogStrKey.insert(strKey_Gn_Track_Title_Official,		"Track_Title_Official");
	g_mapLogStrKey.insert(strKey_Gn_Package_Language_Display,	"Package_Language_Display");
	g_mapLogStrKey.insert(strKey_Gn_Year,						"Year");
	g_mapLogStrKey.insert(strKey_Gn_Genre_Level_1,				"Genre_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Genre_Level_2,				"Genre_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Genre_Level_3,				"Genre_Level_3");
	g_mapLogStrKey.insert(strKey_Gn_Album_Total_In_Set,			"Album_Total_In_Set");
	g_mapLogStrKey.insert(strKey_Gn_Album_Disk_In_Set,			"Album_Disk_In_Set");
	g_mapLogStrKey.insert(strKey_Gn_Album_Track_Count,			"Album_Track_Count");
	g_mapLogStrKey.insert(strKey_Gn_Album_Label,				"Album_Label");
	g_mapLogStrKey.insert(strKey_Gn_Album_Contributor_Name,		"Album_Contributor_Name");
	g_mapLogStrKey.insert(strKey_Gn_Track_Contributor_Name,		"Track_Contributor_Name");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Tls_Name,		"Contributor_Tls_Name");

	g_mapLogStrKey.insert(strKey_Gn_Contributor_Origin_Level_1,				"Contributor_Origin_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Origin_Level_2,				"Contributor_Origin_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Origin_Level_3,				"Contributor_Origin_Level_3");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Origin_Level_4,				"Contributor_Origin_Level_4");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Era_Level_1,				"Contributor_Era_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Era_Level_2,				"Contributor_Era_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Era_Level_3,				"Contributor_Era_Level_3");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_ArtistType_Level_1,			"Contributor_ArtistType_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_ArtistType_Level_2,			"Contributor_ArtistType_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Name,			"Contributor_Collaborator_Name");

	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Origin_Level_1,	"Contributor_Collaborator_Origin_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Origin_Level_2,	"Contributor_Collaborator_Origin_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Origin_Level_3,	"Contributor_Collaborator_Origin_Level_3");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Origin_Level_4,	"Contributor_Collaborator_Origin_Level_4");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Era_Level_1,		"Contributor_Collaborator_Era_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Era_Level_2,		"Contributor_Collaborator_Era_Level_2");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_Era_Level_3,		"Contributor_Collaborator_Era_Level_3");

	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_ArtistType_Level_1,	"Contributor_Collaborator_ArtistType_Level_1");
	g_mapLogStrKey.insert(strKey_Gn_Contributor_Collaborator_ArtistType_Level_2,	"Contributor_Collaborator_ArtistType_Level_2");

	g_mapLogStrKey.insert(strKey_Gn_Track_Number,				"Track_Number");
	g_mapLogStrKey.insert(strKey_Gn_Classical_Data,				"Classical_Data");
	g_mapLogStrKey.insert(strKey_Gn_Collaborator_Result,		"Collaborator_Result");
	g_mapLogStrKey.insert(strKey_Gn_Composition_Form,			"Composition_Form");
	g_mapLogStrKey.insert(strKey_Gn_Discover_Seed,				"Discover_Seed");
	g_mapLogStrKey.insert(strKey_Gn_Mood_Level1,				"Mood_Level1");
	g_mapLogStrKey.insert(strKey_Gn_Mood_Level2,				"Mood_Level2");
	g_mapLogStrKey.insert(strKey_Gn_Tempo_Level1,				"Tempo_Level1");
	g_mapLogStrKey.insert(strKey_Gn_Tempo_Level2,				"Tempo_Level2");
	g_mapLogStrKey.insert(strKey_Gn_Tempo_Level3,				"Tempo_Level3");
	g_mapLogStrKey.insert(strKey_Gn_CoWork_Title_Official,		"CoWork_Title_Official");
	g_mapLogStrKey.insert(strKey_Gn_CoWork_Composition_Form,	"CoWork_Composition_Form");
	g_mapLogStrKey.insert(strKey_Gn_Error,						"Gn_Error");

	g_mapLogStrKey.insert(strKey_PkgPath,		"Pkg Path");
	g_mapLogStrKey.insert(strKey_NewPath,		"New Path");
	g_mapLogStrKey.insert(strKey_VerPath,		"Ver Path");

	g_mapLogStrKey.insert(strKey_AnalyticJson,	"Youtube AnalyticJson");
	g_mapLogStrKey.insert(strKey_RawJson,		"Youtube RawJson");
	g_mapLogStrKey.insert(strKey_JsonForm,		"Json Form");
	g_mapLogStrKey.insert(strKey_Ref,			"Youtube Ref");
	g_mapLogStrKey.insert(strKey_Location,		"Location");

	// Str Keys >>

	// Chr Keys <<
	g_mapLogCharKey.insert(cKey_FreqID,			"Frequency ID");	// for DAB

	g_mapLogCharKey.insert(blKey_Bool,			"Bool");			// Bool
	g_mapLogCharKey.insert(blKey_Album,			"Is Album");
	g_mapLogCharKey.insert(blKey_Copy,			"Is Copy");
	g_mapLogCharKey.insert(blKey_Search,		"Is Search");
	g_mapLogCharKey.insert(blKey_OverWrite,		"Is OverWrite");

	g_mapLogCharKey.insert(blKey_LogoGet,		"Is Log Get");
	g_mapLogCharKey.insert(blKey_UrlSet,		"Is Url Set");

	g_mapLogCharKey.insert(blKey_GetMetaData,	"Is Get Meta Data");
	g_mapLogCharKey.insert(blKey_X10Server,		"Is X10 Server");

	g_mapLogCharKey.insert(blKey_FsType,		"Is FS Type");		// QThreadUdev <<
	g_mapLogCharKey.insert(blKey_Mount,			"Is Mount");		// QThreadUdev >>

	g_mapLogCharKey.insert(blKey_Wl_EncKey,		"Is Wl Enc Key");

	g_mapLogCharKey.insert(blKey_Dsd,			"Is DSD");
	g_mapLogCharKey.insert(blKey_DsdDoP,		"Is DSD DoP");
	g_mapLogCharKey.insert(blKey_ExistArt,		"Is Exist Art");
	g_mapLogCharKey.insert(blKey_NotRecur,		"Not Recur");
	g_mapLogCharKey.insert(blKey_DsdLRSwap,		"Is DSD Swap");

	g_mapLogCharKey.insert(blKey_Programmed,			"Is Programmed");
	g_mapLogCharKey.insert(blKey_AEventSetted,			"Is AEvent Set");
	//g_mapLogCharKey.insert(blKey_AirableSkipSet,		"Is Airable Skip Set");
#if USE_OLD_APPSERVER
	g_mapLogCharKey.insert(blKey_AirableExecuteFromApp,	"Is Airable Execute From App");
#endif	// USE_OLD_APPSERVER
	g_mapLogCharKey.insert(blKey_Raat_Stream_Radio,		"Is Raat Radio");

	g_mapLogCharKey.insert(blKey_Hide,				"Is Hide");

	g_mapLogCharKey.insert(blKey_Dlg,				"Is Dialog");
	g_mapLogCharKey.insert(blKey_New,				"Is New");
	g_mapLogCharKey.insert(blKey_Mqa,				"Is Mqa");
	g_mapLogCharKey.insert(blKey_Shutdown,			"Is Shutdown");
	g_mapLogCharKey.insert(blKey_Enable,			"Is Enable");
	g_mapLogCharKey.insert(blKey_Auto,				"Is Auto");
	g_mapLogCharKey.insert(blKey_Mute,				"Is Mute");

	g_mapLogCharKey.insert(blKey_ARatePositive,		"Is Positive Active");
	g_mapLogCharKey.insert(blKey_ARateNeutral,		"Is Neutral Active");
	g_mapLogCharKey.insert(blKey_ARateNegative,		"Is Negative Active");
	g_mapLogCharKey.insert(blKey_ARateSetted,		"Is Rate Setted");
	g_mapLogCharKey.insert(blKey_QueryForm,			"Is Query Form");
	g_mapLogCharKey.insert(blKey_Remove,			"Is Remove");
	g_mapLogCharKey.insert(blKey_Check,				"Is Check");
	g_mapLogCharKey.insert(blKey_Reboot,			"Is Reboot");
	g_mapLogCharKey.insert(blKey_SmbCa,				"Is Smb CA");
	// Chr Keys >>

	g_blInitLogMapData = true;
}
#endif
