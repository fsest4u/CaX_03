#ifndef __TMAPDATA_H__
#define __TMAPDATA_H__

//#include "HwDef.h"	// temp_code

#include "log.h"

#include "TMapDataDef.h"

#define MapIntAdd(Md, iKey, iValue)			Md.mapInt.insert(iKey, iValue)
#define MapIntVal(Md, iKey)					Md.mapInt.value(iKey)
#define MapIntDel(Md, iKey)					Md.mapInt.remove(iKey)
#define pMapIntAdd(Md, iKey, iValue)		Md->mapInt.insert(iKey, iValue)
#define pMapIntVal(Md, iKey)				Md->mapInt.value(iKey)
#define pMapIntDel(Md, iKey)				Md->mapInt.remove(iKey)

#define dMapIntVal(Md, iKey)				Md.mapInt.value(iKey, INT_MIN)

#define MapStrAdd(Md, iKey, strValue)		Md.mapStr.insert(iKey, strValue)
#define MapStrVal(Md, iKey)					Md.mapStr.value(iKey)
#define MapStrDel(Md, iKey)					Md.mapStr.remove(iKey)
#define pMapStrAdd(Md, iKey, strValue)		Md->mapStr.insert(iKey, strValue)
#define pMapStrVal(Md, iKey)				Md->mapStr.value(iKey)
#define pMapStrDel(Md, iKey)				Md->mapStr.remove(iKey)

#define MapChrAdd(Md, iKey, cValue)			Md.mapChar.insert(iKey, cValue)
#define MapChrVal(Md, iKey)					Md.mapChar.value(iKey)
#define MapChrDel(Md, iKey)					Md.mapChar.remove(iKey)
#define pMapChrAdd(Md, iKey, cValue)		Md->mapChar.insert(iKey, cValue)
#define pMapChrVal(Md, iKey)				Md->mapChar.value(iKey)
#define pMapChrDel(Md, iKey)				Md->mapChar.remove(iKey)

#define Tst_Mask_List				0x00000001
#define Tst_Mask_File				0x00000002
#define Tst_Mask_Net				0x00000004	// Network Streamming
#define Tst_Mask_Input				0x00000008

#define Tst_Mask_Input_Out			0x00000010

#if CA_INPUT

	#if CA_INPUT_USB_DAC
		#define Tst_Mask_UsbDac		0x00000020
	#endif

	#if CA_INPUT_BLUETOOTH
		#define Tst_Mask_Bluetooth	0x00000040
	#endif

	#if CA_FM_RADIO
		#define Tst_Mask_FmRadio	0x00000080
		#define Tst_Mask_Dab		0x00000100
		#define Tst_Mask_DabFmRadio	( Tst_Mask_FmRadio | Tst_Mask_Dab )
	#endif

	#if CA_INPUT_DEVS
		#define Tst_Mask_AES		0x00000200
		#define Tst_Mask_Coaxial	0x00000400
		#define Tst_Mask_Optical	0x00000800
		#define Tst_Mask_LineIn		0x00001000
		#define Tst_Mask_AuxIn		0x00002000
		#define Tst_Mask_PhonoIn	0x00004000

		#define Tst_Mask_Digital	( Tst_Mask_AES | Tst_Mask_Coaxial | Tst_Mask_Optical )
		#define Tst_Mask_Analog		( Tst_Mask_LineIn | Tst_Mask_AuxIn | Tst_Mask_PhonoIn )
	#endif
#endif

#define Tst_Mask_FFMpeg				0x00008000
#define Tst_Mask_NoArt				0x00010000
#define Tst_Mask_NoSeek				0x00020000

#define Tst_Mask_MusicDB			0x00040000
#define Tst_Mask_IRadio				0x00080000
#define Tst_Mask_Cue				0x00100000

#if CA_CD_RIP
	#define Tst_Mask_AudioCd		0x00200000
#endif

#define Tst_Mask_UpnpRenderer		0x00400000
#define Tst_Mask_AirPlay			0x00800000

#define Tst_Mask_Qobuz				0x01000000
#define Tst_Mask_Airable			0x02000000
#define Tst_Mask_Net_Service		0x04000000
#define Tst_Mask_Spotify			0x08000000
#define Tst_Mask_Raat				0x10000000
#define Tst_Mask_Youtube			0x20000000

#if CA_INPUT && CA_INPUT_EARC
	#define Tst_Mask_eARC			0x40000000
#endif

#define Tst_Mask_GroupPlay			0x80000000

#define Tst_None					0x00000000

#define Tst_Input_Out				( Tst_Mask_Input | Tst_Mask_Input_Out )

#if CA_INPUT

	#if CA_FM_RADIO
		#define Tst_FmRadio		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_FmRadio )
		#define Tst_Dab			( Tst_Mask_Input | Tst_Mask_NoSeek | Tst_Mask_Dab )
	#endif

	#if CA_INPUT_DEVS
		#define Tst_AES			( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_AES )
		#define Tst_Coaxial		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_Coaxial )
		#define Tst_Optical		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_Optical )
		#define Tst_LineIn		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_LineIn )
		#define Tst_AuxIn		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_AuxIn )
		#define Tst_PhonoIn		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_PhonoIn )
	#endif

	#if CA_INPUT_USB_DAC
		#define Tst_UsbDac		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_UsbDac )
	#endif
	#if CA_INPUT_BLUETOOTH
		#define Tst_Bluetooth	( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_Bluetooth )
	#endif
	#if CA_INPUT_EARC
		#define Tst_eARC		( Tst_Mask_Input | Tst_Mask_NoArt | Tst_Mask_NoSeek | Tst_Mask_eARC )
	#endif
#endif

#define Tst_File				( Tst_Mask_File | Tst_Mask_List | Tst_Mask_FFMpeg )
#define Tst_MusicDB				( Tst_Mask_MusicDB | Tst_Mask_File | Tst_Mask_List | Tst_Mask_FFMpeg )

#if CA_CD_RIP
	#define Tst_AudioCd			( Tst_Mask_AudioCd | Tst_Mask_NoArt | Tst_Mask_List )
#endif

#define Tst_CueFile				( Tst_Mask_Cue | Tst_Mask_File | Tst_Mask_List | Tst_Mask_FFMpeg )

#define Tst_IRadio				( Tst_Mask_Net | Tst_Mask_IRadio | Tst_Mask_NoSeek | Tst_Mask_FFMpeg )

#define Tst_UpnpRenderer		( Tst_Mask_Net | Tst_Mask_UpnpRenderer | Tst_Mask_FFMpeg )
#define Tst_AirPlay				( Tst_Mask_Net | Tst_Mask_NoSeek | Tst_Mask_AirPlay )
#define Tst_NetList				( Tst_Mask_Net | Tst_Mask_List | Tst_Mask_FFMpeg )
#define Tst_UpnpMusicDB			( Tst_Mask_MusicDB | Tst_Mask_File | Tst_Mask_UpnpRenderer | Tst_Mask_FFMpeg )

#define Tst_Qobuz				( Tst_NetList | Tst_Mask_Net_Service | Tst_Mask_Qobuz )
#define Tst_Airable				( Tst_NetList | Tst_Mask_Net_Service | Tst_Mask_Airable )
#define Tst_AirableIRadio		( Tst_IRadio | Tst_Mask_Net_Service | Tst_Mask_Airable )
#define Tst_Spotify				( Tst_Mask_Net | Tst_Mask_Spotify )
#define Tst_Raat				( Tst_Mask_Net | Tst_Mask_Raat )
#define Tst_Youtube				( Tst_NetList | Tst_Mask_Net_Service | Tst_Mask_Youtube )
#define Tst_GroupPlay			( Tst_Mask_Net | Tst_Mask_NoSeek | Tst_Mask_GroupPlay )

#define Tsst_AirableFeed			1
#define Tsst_AirableUpnp			2
#define Tsst_AirableTidal			3
#define Tsst_AirableNapster			4
#define Tsst_AirableDeezer			5
#define Tsst_AirableHighResAudio	6
#define Tsst_AirableAmazon			7

typedef enum
{
	iKey_Type = 1,		// Tst_???
	iKey_SubType,		// Tsst_???

	iKey_ID,
	iKey_ArtistID,
	iKey_AlbumArtistID,
	iKey_GenreID,
	iKey_AlbumID,
	iKey_ComposerID,
	iKey_MoodID,
	iKey_FolderID,
	iKey_ScanPathID,
	iKey_FilePathID,
	iKey_SubID,

	iKey_FsUuid,
	iKey_Track,
	iKey_Year,
	iKey_CdYear,
	iKey_CdTotal,
	iKey_CdNumber,
	iKey_NewCdNumber,

	iKey_Duration,
	iKey_DurationMs,
	iKey_SecMs,
	iKey_BitRate,
	iKey_SampleRate,
	iKey_Channel,

	iKey_SampleCnt,
	iKey_BitsPerSample,
	iKey_BytesPerSample,
	iKey_CodecId,

	iKey_Int,			// int value
	iKey_Seq,			// Sequence
	iKey_Speed,
	iKey_Port,
	iKey_Major,
	iKey_Minor,
	iKey_StatusCode,
	iKey_Format,
	iKey_Sec,

	iKey_Pointer,				// Pointer variable
	iKey_PointerThis,			// Pointer variable
	iKey_PointerStringList,		// Pointer variable
	iKey_PointerMapDataList,	// Pointer variable
	iKey_PointerIntList,		// Pointer variable

	iKey_FirstSector,
	iKey_LastSector,

	iKey_Bus,					// QThreadUdev <<
	iKey_IfType,				// enumNetworkConnection : NC_NONE, NC_WIRED, NC_WIRELESS
	iKey_CdMedia,				// QThreadUdev >>	// iCdMedia_None, iCdMedia_Audio, iCdMedia_Data

	iKey_Menu,					// QwPlayMenu <<
	iKey_MenuDepth,				// 1, 2, 3, ...
	iKey_Select,				// Curr. selected index
	iKey_Sort,
	iKey_Table,					// iTable_???		// CoverView
	iKey_Error,
	iKey_View,
	iKey_ViewKey,				// QwPlayMenu >>

	iKey_Wl_Channel,			// Wireless Keys <<
	iKey_Wl_Sensitivity,
	iKey_Wl_Encryption,
	iKey_Wl_SupportedEncryption,
	iKey_Wl_Mode,
	iKey_Wl_Setup,
	iKey_Wl_Security,
	iKey_Wl_SupportedSecurity,	// Wireless Keys >>

	iKey_FrameSize,
	iKey_SampleSize,
	iKey_CtlPort,

	iKey_Freq,			// for Radio
	iKey_SvcID,			// for DAB
	iKey_CompID,		// for DAB
	iKey_SvcCompType,	// for DAB
	iKey_EnsembleID,	// for DAB

	iKey_PcmMask,
	iKey_SampleRateMask,
	iKey_VendorID,
	iKey_ModelID,

	iKey_TrID,			// AppServer <<
	iKey_AppDoType,
	iKey_PageIndex,
	iKey_PageSize,		// AppServer >>

	iKey_Load_Priority,
	iKey_RaidMode,

	iKey_TimeStemp,
	iKey_Index,
	iKey_Count,
	iKey_Volume,

	iKey_ReplayGain,
	iKey_ReplayPeak,

	iKey_New,
	iKey_Curr,
	iKey_Src,
	iKey_Dst,
	iKey_UrlIndex,
	iKey_EventID,
	iKey_FileManager,

	iKey_PlaylistType,		// Youtube << >>
	iKey_Size,
	iKey_Retry,
	iKey_Flag,

	iKey_PointerMdList,
	iKey_PointerMdImages,
	iKey_PointerMdActions,

	iKey_LpLsInt,
	iKey_LpLsStr,
	iKey_LpLsStrStr,
} EMapDataKeyInt;

typedef enum
{
	strKey_Type = 1,
	strKey_Url,
	strKey_OrgUrl,
	strKey_NextUrl,
	strKey_FilePath,
	strKey_Disp,
	strKey_ID,
	strKey_String,
	strKey_Text,

	strKey_Name,
	strKey_Title,
	strKey_TitleTop,
	strKey_StreamTitle,

	strKey_Artist,
	strKey_AlbumArtist,
	strKey_Genre,
	strKey_Album,
	strKey_Composer,
	strKey_Mood,
	strKey_Folder,

	strKey_Track,
	strKey_Year,
	strKey_Tempo,
	strKey_Format,
	strKey_CdToc,

	strKey_CoverArtPath,
	strKey_AlbumArtUrl,
	strKey_LogoUrl,
	strKey_LogoType,

	strKey_Ext,
	strKey_Codec,
	strKey_Search,
	strKey_TableName,
	strKey_Uuid,
	strKey_Host,
	strKey_Path,
	strKey_FileName,
	strKey_Comment,
	strKey_Nickname,
	strKey_UncPath,
	strKey_RelPath,		// Path from MntPath

	strKey_MacAddr,
	strKey_IpAddr,
	strKey_NetMask,
	strKey_Gateway,
	strKey_DnsAddr,

	strKey_Prog,		// QThreadUdev <<
	strKey_Option,
	strKey_DevPath,
	strKey_DevName,
	strKey_FsType,
	strKey_MntName,
	strKey_MntPath,
	strKey_Action,
	strKey_If,
	strKey_FsUuid,		// QThreadUdev >>

	strKey_Wl_AP,		// Wireless Keys <<
	strKey_Wl_WEP,		// Wireless Keys >>

	strKey_Schema,
	strKey_Version,
	strKey_Method,
	strKey_ReasonPhrase,
	strKey_DacpId,
	strKey_ActiveRemote,
	strKey_Domain,

	strKey_Res,
	strKey_Workgroup,
	strKey_Server,
	strKey_User,
	strKey_Password,
	strKey_Share,
	strKey_OldName,
	strKey_Value,
	strKey_ArtistImageUrl,

	strKey_Id_Path,
	strKey_Category,
	strKey_Site,
	strKey_InPath,
	strKey_OutPath,
	strKey_State,
	strKey_CardID,
	strKey_AudioFormat,

	strKey_AFavoriteUrl,				// Airable <<
	strKey_AEventStartUrl,
	strKey_AEventStopUrl,
	strKey_AEventErrorUrl,
	strKey_AEventPauseUrl,
	//strKey_ASkipStatusUrl,
	strKey_AEventTitle,
	stkKey_AEventLabel,
	strKey_AEventBtnYes,
	strKey_AEventBtnCancel,
	strKey_AEventBtnYesUrl,
	strKey_AEventBtnCancelUrl,
	strKey_ARatePositiveUrl,
	strKey_ARateNeutralUrl,
	strKey_ARateNegativeUrl,			// Airable >>
	strKey_RaatOneLine,					// Raat <<
	strKey_RaatTwoLineTitle,
	strKey_RaatTwoLineSubtitle,
	strKey_RaatThreeLineTitle,
	strKey_RaatThreeLineSubTitle,
	strKey_RaatThreeLineSubSubTitle,	// Raat >>

	strKey_Param1,

	//Gracenote SDK
	strKey_Gn_TUI,
	strKey_Gn_TUI_TAG,

	// Album
	strKey_Gn_Album_Title_Official,			//strKey_Album(Album's Title)
	strKey_Gn_Package_Language_Display,
	strKey_Gn_Year,							//iKey_Year
	strKey_Gn_Genre_Level_1,				//strKey_Genre : Album/Track
	strKey_Gn_Genre_Level_2,
	strKey_Gn_Genre_Level_3,
	strKey_Gn_Album_Total_In_Set,			//iKey_CdTotal
	strKey_Gn_Album_Disk_In_Set,			//iKey_CdNumber
	strKey_Gn_Album_Track_Count,			//iKey_Track : Total Track Count
	strKey_Gn_Album_Label,

	strKey_Gn_Album_Contributor_Name,		//strKey_Artist : Album
	strKey_Gn_Track_Contributor_Name,		//strKey_Artist : Track

	strKey_Gn_Contributor_Tls_Name,
	strKey_Gn_Contributor_Origin_Level_1,
	strKey_Gn_Contributor_Origin_Level_2,
	strKey_Gn_Contributor_Origin_Level_3,
	strKey_Gn_Contributor_Origin_Level_4,
	strKey_Gn_Contributor_Era_Level_1,
	strKey_Gn_Contributor_Era_Level_2,
	strKey_Gn_Contributor_Era_Level_3,
	strKey_Gn_Contributor_ArtistType_Level_1,
	strKey_Gn_Contributor_ArtistType_Level_2,

	strKey_Gn_Contributor_Collaborator_Name,
	strKey_Gn_Contributor_Collaborator_Origin_Level_1,
	strKey_Gn_Contributor_Collaborator_Origin_Level_2,
	strKey_Gn_Contributor_Collaborator_Origin_Level_3,
	strKey_Gn_Contributor_Collaborator_Origin_Level_4,
	strKey_Gn_Contributor_Collaborator_Era_Level_1,
	strKey_Gn_Contributor_Collaborator_Era_Level_2,
	strKey_Gn_Contributor_Collaborator_Era_Level_3,
	strKey_Gn_Contributor_Collaborator_ArtistType_Level_1,
	strKey_Gn_Contributor_Collaborator_ArtistType_Level_2,

	strKey_Gn_Track_Title_Official,		//strKey_Title(Track's Title)
	strKey_Gn_Track_Number,				//iKey_Track
	strKey_Gn_Classical_Data,
	strKey_Gn_Collaborator_Result,
	strKey_Gn_Composition_Form,
	strKey_Gn_Discover_Seed,
	strKey_Gn_Mood_Level1,				//strKey_Mood
	strKey_Gn_Mood_Level2,
	strKey_Gn_Tempo_Level1,
	strKey_Gn_Tempo_Level2,
	strKey_Gn_Tempo_Level3,				//strKey_Tempo

	strKey_Gn_CoWork_Title_Official,
	strKey_Gn_CoWork_Composition_Form,

	strKey_Gn_Error,

	strKey_PkgPath,				// HttpUpdateWaitDlg <<
	strKey_NewPath,
	strKey_VerPath,				// HttpUpdateWaitDlg >>

	strKey_AnalyticJson,		// Youtube <<
	strKey_RawJson,
	strKey_JsonForm,
	strKey_Ref,					// Youtube >>
	strKey_Location,
} EMapDataKeyStr;

typedef enum
{
	cKey_FreqID = 1,		// for DAB

	blKey_Bool,				// Bool
	blKey_Album,
	blKey_Copy,
	blKey_Search,
	blKey_OverWrite,

	blKey_LogoGet,
	blKey_UrlSet,

	blKey_GetMetaData,
	blKey_X10Server,

	blKey_FsType,			// QThreadUdev <<
	blKey_Mount,			// QThreadUdev >>

	blKey_Wl_EncKey,		// Wireless Key

	blKey_Dsd,
	blKey_DsdDoP,
	blKey_ExistArt,
	blKey_NotRecur,
	blKey_DsdLRSwap,

	blKey_Programmed,		// Airable_Programmed_Types
	blKey_AEventSetted,
	//blKey_AirableSkipSet,
#if USE_OLD_APPSERVER
	blKey_AirableExecuteFromApp,
#endif	// USE_OLD_APPSERVER
	blKey_Raat_Stream_Radio,

	blKey_MutePlay,
	blKey_Hide,

	blKey_Dlg,
	blKey_New,
	blKey_Mqa,
	blKey_Shutdown,
	blKey_Enable,
	blKey_Auto,
	blKey_Mute,

	blKey_ARatePositive,	// Airable <<
	blKey_ARateNeutral,
	blKey_ARateNegative,
	blKey_ARateSetted,		// Airable >>
	blKey_QueryForm,
	blKey_Remove,
	blKey_Check,
	blKey_Reboot,
	blKey_SmbCa,
} EMapDataKeyChr;

#define cVal_True		0x01
#define cVal_False		0x00

bool	MapStrSet(TMapData &Md, int iStrKey, QString strValue, bool &blChange);
bool	MapIntSet(TMapData &Md, int iIntKey, int iValue, bool &blChange);
bool	MapBoolSet(TMapData &Md, int iChrKey, bool blValue, bool &blChange);

void	MapDataMergy(TMapData &MdTo, TMapData &MdFrom);
bool	SetMapDataIfEmpty(int iStrKey, QString strValue, TMapData &MapData);
bool	SetMapDataIfNotEmpty(int iStrKey, QString strValue, TMapData &MapData);
bool	ReplaceMapDataList(TMapDataList &lsMapData, int index, int iStrKey, QString str);
int		FindMapDataListFromStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey);			// Get MapData from StrKey
int		FindMapDataList(TMapDataList &lsMapData, TMapData &MdData, int iKey, int64_t llValue);		// Get MapData from iKey & Value
int		FindMapDataList(TMapDataList &lsMapData, TMapData &MdData, int iStrKey, QString strValue);	// Get MapData from StrKey & Value
bool	AppendMapDataListCheckStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey);		// StrKey is not exist then append
bool	RemoveMapDataListFromStrKey(TMapDataList &lsMapData, TMapData &MdData, int iStrKey);		// StrKey is exist then Remove & Get Removed MapData
void	ClearMapData(TMapData *lpMapData);

#ifndef NO_LOGGING
	#include <QString>

	#include "StringLib.h"

	void	_LogMapData(TMapData MdInfo, QString strName);

	#define	LogMapData(a, b)																		\
			{																						\
				LoggingWarning("******************** MapData : %s\n", ToAscii(QString(b)).data());	\
				_LogMapData(a, b);																	\
			}
#else
	#define	LogMapData(a, b)
#endif

#endif	// __TMAPDATA_H__
