#include <QIcon>

#include "utilnovatron.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/browser.h"

UtilNovatron::UtilNovatron()
{

}

QIcon UtilNovatron::GetMenuIcon(QString value)
{
	if (value.contains(STR_ADD))
	{
		return QIcon(":/resource/browser-icon16-newfolder@3x.png");
	}
	else if (value.contains(STR_ADD_TO_PLAYLIST))
	{
		return QIcon(":/resource/play-popup-icon16-addplaylist@3x.png");
	}
	else if (value.contains(STR_CD_RIPPING))
	{
		return QIcon(":/resource/audiocd-popup-icon16-cdrip@3x.png");
	}
	else if (value.contains(STR_CLEAR_ALL))
	{
		return QIcon(":/resource/playlist-icon16-clearall@3x.png");
	}
	else if (value.contains(STR_CONVERT_FORMAT))
	{
		return QIcon(":/resource/browser-icon16-convertaudio@3x.png");
	}
	else if (value.contains(STR_COPY))
	{
		return QIcon(":/resource/browser-icon16-copy@3x.png");
	}
	else if (value.contains(STR_DELETE))
	{
		return QIcon(":/resource/playlist-icon16-delete@3x.png");
	}
	else if (value.contains(STR_DELETE_TO_PLAYLIST))
	{
		return QIcon("");
	}
	else if (value.contains(STR_EDIT_TAG))
	{
		return QIcon(":/resource/browser-icon16-settag@3x.png");
	}
	else if (value.contains(STR_EJECT_CD))
	{
		return QIcon(":/resource/audiocd-popup-icon16-eject@3x.png");
	}
	else if (value.contains(STR_EXPORT_TRACK))
	{
		return QIcon(":/resource/playlist-icon16-exportsong@3x.png");
	}
	else if (value.contains(STR_GAIN_SET))
	{
		return QIcon(":/resource/play-popup-icon16-replaygain@3x.png");
	}
	else if (value.contains(STR_GAIN_CLEAR))
	{
		return QIcon(":/resource/play-popup-icon16-cleanreplaygain@3x.png");
	}
	else if (value.contains(STR_INFO))
	{
		return QIcon(":/resource/browser-icon16-infoalbum@3x.png");
	}
	else if (value.contains(STR_LOAD_COUNT))
	{
		return QIcon(":/resource/play-popup-icon16-loadalloff@3x.png");
	}
	else if (value.contains(STR_MOVE))
	{
		return QIcon(":/resource/browser-icon16-move@3x.png");
	}
	else if (value.contains(STR_OPTION_PLAY_SUBDIR))
	{
		return QIcon("");
	}
	else if (value.contains(STR_OPTION_OVERWRITE))
	{
		return QIcon("");
	}
	else if (value.contains(STR_PLAY_NOW))
	{
		return QIcon(":/resource/audiocd-popup-icon16-play@3x.png");
	}
	else if (value.contains(STR_PLAY_LAST))
	{
		return QIcon("");
	}
	else if (value.contains(STR_PLAY_NEXT))
	{
		return QIcon("");
	}
	else if (value.contains(STR_PLAY_CLEAR))
	{
		return QIcon("");
	}
	else if (value.contains(STR_RELOAD))
	{
		return QIcon(":/resource/play-popup-icon16-reload@3x.png");
	}
	else if (value.contains(STR_RENAME))
	{
		return QIcon(":/resource/play-popup-icon16-nameedit@3x.png");
	}
	else if (value.contains(STR_RESERVE_RECORD_LIST))
	{
		return QIcon("");
	}
	else if (value.contains(STR_SEARCH_ALL_N_DELETE))
	{
		return QIcon("");
	}
	else if (value.contains(STR_SEARCH_ALL))
	{
		return QIcon("");
	}
	else if (value.contains(STR_SEARCH_COVERART))
	{
		return QIcon(":/resource/play-popup-icon16-searchcoverart@3x.png");
	}
	else if (value.contains(STR_SELECT_ALL))
	{
		return QIcon(":/resource/play-popup-icon16-selectall@3x.png");
	}
	else if (value.contains(STR_SEARCH_COVERART))
	{
		return QIcon(":/resource/play-popup-icon16-searchcoverart@3x.png");
	}
	// side menu
	else if (value.contains(STR_MUSIC_DB))
	{
		return QIcon(":/resource/submenu-icon40-musicdb@3x.png");
	}
	else if (value.contains(STR_AUDIO_CD))
	{
		return QIcon(":/resource/submenu-icon40-audiocd@3x.png");
	}
	else if (value.contains(STR_PLAYLIST))
	{
		return QIcon(":/resource/submenu-icon40-playlist@3x.png");
	}
	else if (value.contains(STR_BROWSER))
	{
		return QIcon(":/resource/submenu-icon40-browser@3x.png");
	}
	else if (value.contains(STR_ISERVICE))
	{
		return QIcon(":/resource/submenu-icon40-internetservice@3x.png");
	}
	else if (value.contains(STR_INPUT))
	{
		return QIcon(":/resource/submenu-icon40-input@3x.png");
	}
	else if (value.contains(STR_FM_RADIO))
	{
		return QIcon(":/resource/submenu-icon40-fmradio@3x.png");
	}
	else if (value.contains(STR_DAB_RADIO))
	{
		return QIcon(":/resource/submenu-icon40-davradio@3x.png");
	}
	else if (value.contains(STR_GROUP_PLAY))
	{
		return QIcon(":/resource/submenu-icon40-groupplay@3x.png");
	}
	else if (value.contains(STR_SETUP))
	{
		return QIcon(":/resource/submenu-icon40-setup@3x.png");
	}
	else if (value.contains(STR_SELECT_DEVICE))
	{
		return QIcon(":/resource/submenu-icon40-selectdevice@3x.png");
	}
	else if (value.contains(STR_POWER_ON))
	{
		return QIcon(":/resource/submenu-icon40-wolpoweron@3x.png");
	}
	else if (value.contains(STR_POWER_OFF))
	{
		return QIcon(":/resource/submenu-icon40-poweroff@3x.png");
	}
	else if (value.contains(STR_ABOUT))
	{
		return QIcon(":/resource/submenu-icon40-aboutmusicx@3x.png");
	}
	else
	{
		LogDebug("There is no icon corresponding to the string.");
		return QIcon("");
	}
}

QString UtilNovatron::GetCoverArtIcon(const int service, const QString value)
{
	QString coverArt = "";

	if (service == SIDEMENU_MUSIC_DB)
	{
		coverArt = "";

	}
	else if (service == SIDEMENU_AUDIO_CD)
	{
		coverArt = ":/resource/playlist-img160-albumart-h@3x.png";

	}
	else if (service == SIDEMENU_PLAYLIST)
	{
		coverArt = "";

	}
	else if (service == SIDEMENU_BROWSER)
	{
		if (value.contains("HDD1"))
		{
			coverArt = ":/resource/browser-img160-hdd-n@3x.png";
		}
		else if (value.contains("NET"))
		{
			coverArt = ":/resource/browser-img160-net-n@3x.png";
		}
		else if (value.contains("UPnP"))
		{
			coverArt = ":/resource/browser-img160-upnp-n@3x.png";
		}
		else
		{
			coverArt = ":/resource/browser-img160-hdd-n@3x.png";
		}
	}
	else if (service == SIDEMENU_ISERVICE)
	{
		coverArt = "";

	}
	else if (service == SIDEMENU_INPUT)
	{
		if (value.contains("AES/EBU"))
		{
			coverArt = ":/resource/input-img160-aesebu-n@3x.png";
		}
		else if (value.contains("COAXIAL"))
		{
			coverArt = ":/resource/input-img160-coaxial-n@3x.png";
		}
		else if (value.contains("TOSLINK"))
		{
			coverArt = ":/resource/input-img160-toslink-n@3x.png";
		}
		else if (value.contains("ANALOG IN"))
		{
			coverArt = ":/resource/input-img160-analogin-n@3x.png";
		}
		else if (value.contains("AUX IN"))
		{
			coverArt = ":/resource/input-img160-auxin-n@3x.png";
		}
		else if (value.contains("PHONO IN"))
		{
			coverArt = ":/resource/input-img160-ponoin-n@3x.png";
		}
	}
	else if (service == SIDEMENU_FM_RADIO)
	{
		coverArt = ":/resource/radio-img160-channelicon-n@3x.png";
	}
	else if (service == SIDEMENU_DAB_RADIO)
	{
		coverArt = ":/resource/radio-img160-channelicon-n@3x.png";
	}
	else if (service == SIDEMENU_GROUP_PLAY)
	{
		coverArt = ":/resource/groupp-img160-n@3x.png";

	}
	else if (service == SIDEMENU_SETUP)
	{
		coverArt = "";

	}

	if (coverArt.isEmpty())
	{
		LogDebug("There is no icon corresponding to the string.");
	}

	return coverArt;
}

void UtilNovatron::DebugTypeForBrowser(int type)
{
	if (type & iFolderType_Mask_Play_Top)
	{
		LogDebug("Play_Top");
	}
	if (type & iFolderType_Mask_Play_Option)
	{
		LogDebug("Play_Option");
	}
	if (type & iFolderType_Mask_Play_Check)
	{
		LogDebug("Play_Check");
	}
	if (type & iFolderType_Mask_Play_Select)
	{
		LogDebug("Mask_Play_Select");
	}
	if (type & iFolderType_Mask_Check)
	{
		LogDebug("Mask_Check");
	}
	if (type & iFolderType_Mask_FileMgr)
	{
		LogDebug("Mask_FileMgr");
	}
	if (type & iFolderType_Mask_ReadOnly)
	{
		LogDebug("Mask_ReadOnly");
	}
	if (type & iFolderType_Mask_Sub)
	{
		LogDebug("Mask_Sub");
	}
	if (type & iFolderType_Mask_Dev)
	{
		LogDebug("Mask_Dev");
	}
	if (type & iFolderType_Mask_Hdd)
	{
		LogDebug("Mask_Hdd");
	}
	if (type & iFolderType_Mask_Usb)
	{
		LogDebug("Mask_Usb");
	}
	if (type & iFolderType_Mask_Cd)
	{
		LogDebug("Mask_Cd");
	}
	if (type & iFolderType_Mask_Net)
	{
		LogDebug("Mask_Net");
	}
	if (type & iFolderType_Mask_Upnp)
	{
		LogDebug("Mask_Upnp");
	}
	if (type & iFolderType_Mask_Root)
	{
		LogDebug("Mask_Root");
	}
	if (type & iFolderType_Mask_Dir)
	{
		LogDebug("Mask_Dir");
	}
	if (type & iFolderType_Mask_Song)
	{
		LogDebug("Mask_Song");
	}
	if (type & iFolderType_Mask_Pls)
	{
		LogDebug("Mask_Pls");
	}
	if (type & iFolderType_Mask_Cue)
	{
		LogDebug("Mask_Cue");
	}
	if (type & iFolderType_Mask_IRadio)
	{
		LogDebug("Mask_IRadio");
	}
	if (type & iFolderType_Mask_IsoFile)
	{
		LogDebug("Mask_IsoFile");
	}
	if (type & iFolderType_Mask_IsoRoot)
	{
		LogDebug("Mask_IsoRoot");
	}
	if (type & iFolderType_Mask_Image)
	{
		LogDebug("Mask_Image");
	}
	if (type & iFolderType_Mask_File)
	{
		LogDebug("Mask_File");
	}
	if (type & iFolderType_Mask_Scan)
	{
		LogDebug("Mask_Scan");
	}
	if (type & iFolderType_Mask_Media)
	{
		LogDebug("Mask_Media");
	}
	if (type & iFolderType_Mask_FilePath)
	{
		LogDebug("Mask_FilePath");
	}
}

