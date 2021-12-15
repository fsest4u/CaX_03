#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QStandardPaths>

#include "utilnovatron.h"

#include "util/caxconstants.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/airable.h"
#include "widget/browser.h"
#include "widget/qobuz.h"

UtilNovatron::UtilNovatron()
{

}

QIcon UtilNovatron::GetMenuIcon(QString value)
{
	if (value.contains(STR_ADD))
	{
		return QIcon(":/resource/browser-icon16-newfolder.png");
	}
	else if (value.contains(STR_ADD_TO_PLAYLIST))
	{
		return QIcon(":/resource/play-popup-icon16-addplaylist.png");
	}
	else if (value.contains(STR_CD_RIPPING))
	{
		return QIcon(":/resource/audiocd-popup-icon16-cdrip.png");
	}
	else if (value.contains(STR_CLEAR_ALL))
	{
		return QIcon(":/resource/playlist-icon16-clearall.png");
	}
	else if (value.contains(STR_CONVERT_FORMAT))
	{
		return QIcon(":/resource/browser-icon16-convertaudio.png");
	}
	else if (value.contains(STR_COPY))
	{
		return QIcon(":/resource/browser-icon16-copy.png");
	}
	else if (value.contains(STR_DELETE))
	{
		return QIcon(":/resource/playlist-icon16-delete.png");
	}
	else if (value.contains(STR_DELETE_TO_PLAYLIST))
	{
		return QIcon("");
	}
	else if (value.contains(STR_EDIT_TAG))
	{
		return QIcon(":/resource/browser-icon16-settag.png");
	}
	else if (value.contains(STR_EJECT_CD))
	{
		return QIcon(":/resource/audiocd-popup-icon16-eject.png");
	}
	else if (value.contains(STR_EXPORT_TRACK))
	{
		return QIcon(":/resource/playlist-icon16-exportsong.png");
	}
	else if (value.contains(STR_GAIN_SET))
	{
		return QIcon(":/resource/play-popup-icon16-replaygain.png");
	}
	else if (value.contains(STR_GAIN_CLEAR))
	{
		return QIcon(":/resource/play-popup-icon16-cleanreplaygain.png");
	}
	else if (value.contains(STR_INFO))
	{
		return QIcon(":/resource/browser-icon16-infoalbum.png");
	}
	else if (value.contains(STR_LOAD_COUNT))
	{
		return QIcon(":/resource/play-popup-icon16-loadalloff.png");
	}
	else if (value.contains(STR_MOVE))
	{
		return QIcon(":/resource/browser-icon16-move.png");
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
		return QIcon(":/resource/browser-icon16-playnow.png");
	}
	else if (value.contains(STR_PLAY_LAST))
	{
		return QIcon(":/resource/browser-icon16-playlast.png");
	}
	else if (value.contains(STR_PLAY_NEXT))
	{
		return QIcon(":/resource/browser-icon16-playnext.png");
	}
	else if (value.contains(STR_PLAY_CLEAR))
	{
		return QIcon(":/resource/browser-icon16-clearreplay.png");
	}
	else if (value.contains(STR_RELOAD))
	{
		return QIcon(":/resource/play-popup-icon16-reload.png");
	}
	else if (value.contains(STR_RENAME))
	{
		return QIcon(":/resource/play-popup-icon16-nameedit.png");
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
		return QIcon(":/resource/play-popup-icon16-searchcoverart.png");
	}
	else if (value.contains(STR_SELECT_ALL))
	{
		return QIcon(":/resource/play-popup-icon16-selectall.png");
	}
	else if (value.contains(STR_SEARCH_COVERART))
	{
		return QIcon(":/resource/play-popup-icon16-searchcoverart.png");
	}
	// side menu
	else if (value.contains(STR_MUSIC_DB))
	{
		return QIcon(":/resource/submenu-icon40-musicdb.png");
	}
	else if (value.contains(STR_AUDIO_CD))
	{
		return QIcon(":/resource/submenu-icon40-audiocd.png");
	}
	else if (value.contains(STR_PLAYLIST))
	{
		return QIcon(":/resource/submenu-icon40-playlist.png");
	}
	else if (value.contains(STR_BROWSER))
	{
		return QIcon(":/resource/submenu-icon40-browser.png");
	}
	else if (value.contains(STR_ISERVICE))
	{
		return QIcon(":/resource/submenu-icon40-internetservice.png");
	}
	else if (value.contains(STR_INPUT))
	{
		return QIcon(":/resource/submenu-icon40-input.png");
	}
	else if (value.contains(STR_FM_RADIO))
	{
		return QIcon(":/resource/submenu-icon40-fmradio.png");
	}
	else if (value.contains(STR_DAB_RADIO))
	{
		return QIcon(":/resource/submenu-icon40-davradio.png");
	}
	else if (value.contains(STR_GROUP_PLAY))
	{
		return QIcon(":/resource/submenu-icon40-groupplay.png");
	}
	else if (value.contains(STR_SETUP))
	{
		return QIcon(":/resource/submenu-icon40-setup.png");
	}
	else if (value.contains(STR_SELECT_DEVICE))
	{
		return QIcon(":/resource/submenu-icon40-selectdevice.png");
	}
	else if (value.contains(STR_POWER_ON))
	{
		return QIcon(":/resource/submenu-icon40-wolpoweron.png");
	}
	else if (value.contains(STR_POWER_OFF))
	{
		return QIcon(":/resource/submenu-icon40-poweroff.png");
	}
	else if (value.contains(STR_ABOUT))
	{
		return QIcon(":/resource/submenu-icon40-aboutmusicx.png");
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
		coverArt = ":/resource/playlist-img160-albumart-h.png";

	}
	else if (service == SIDEMENU_PLAYLIST)
	{
		coverArt = "";

	}
	else if (service == SIDEMENU_BROWSER)
	{
		if (value.contains("HDD1"))
		{
			coverArt = ":/resource/browser-img160-hdd-n.png";
		}
		else if (value.contains("NET"))
		{
			coverArt = ":/resource/browser-img160-net-n.png";
		}
		else if (value.contains("UPnP"))
		{
			coverArt = ":/resource/browser-img160-upnp-n.png";
		}
		else
		{
			coverArt = ":/resource/browser-img160-folder-n.png";
		}
	}
	else if (service == SIDEMENU_ISERVICE)
	{
		if (value.contains("rec_list"))
		{
			coverArt = ":/resource/internets-icon40-reservedrecordlist.png";
		}
		else if (value.contains("search"))
		{
			coverArt = ":/resource/internets-icon40-search.png";
		}
		else
		{
			LogDebug("value [%s]", value.toUtf8().data());
			coverArt = ":/resource/internets-icon40-titlecover.png";
		}
	}
	else if (service == SIDEMENU_INPUT)
	{
		if (value.contains(SRC_AES_EBU))
		{
			coverArt = ":/resource/input-img160-aesebu-n.png";
		}
		else if (value.contains(SRC_COAXIAL))
		{
			coverArt = ":/resource/input-img160-coaxial-n.png";
		}
		else if (value.contains(SRC_TOSLINK))
		{
			coverArt = ":/resource/input-img160-toslink-n.png";
		}
		else if (value.contains(SRC_ANALOG_IN))
		{
			coverArt = ":/resource/input-img160-analogin-n.png";
		}
		else if (value.contains(SRC_AUX_IN))
		{
			coverArt = ":/resource/input-img160-auxin-n.png";
		}
		else if (value.contains(SRC_PHONO_IN))
		{
			coverArt = ":/resource/input-img160-ponoin-n.png";
		}
	}
	else if (service == SIDEMENU_FM_RADIO)
	{
		coverArt = ":/resource/radio-img160-channelicon-n.png";
	}
	else if (service == SIDEMENU_DAB_RADIO)
	{
		coverArt = ":/resource/radio-img160-channelicon-n.png";
	}
	else if (service == SIDEMENU_GROUP_PLAY)
	{
		coverArt = ":/resource/groupp-img160-n.png";

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

QString UtilNovatron::GetCoverArtIcon(const int service, const int value)
{
	QString coverArt = "";

	if (service == SIDEMENU_ISERVICE)
	{
		if (value == iIServiceType_Qobuz)
		{
			coverArt = ":/resource/internets-img160-qobuz-n.png";
		}
		else if (value == iIServiceType_Radios)
		{
			coverArt = ":/resource/internets-img160-airable-n.png";
		}
		else if (value == iIServiceType_Podcasts)
		{
			coverArt = ":/resource/internets-img160-podcast-n.png";
		}
		else if (value == iIServiceType_Tidal)
		{
			coverArt = ":/resource/internets-img160-tidal-n.png";
		}
		else if (value == iIServiceType_Deezer)
		{
			coverArt = ":/resource/internets-img160-deezer-n.png";
		}
		else if (value == iIServiceType_Napster)
		{
			coverArt = ":/resource/internets-img160-napster-n.png";
		}
		else if (value == iIServiceType_HighResAudio)
		{
			coverArt = ":/resource/internets-img160-highresaudio-n.png";
		}
		else if (value == iIServiceType_AmazonMusic)
		{
			coverArt = ":/resource/internets-img160-amazonmusic-n.png";
		}
	}

	return coverArt;
}

QString UtilNovatron::CalcSecondToHMS(int seconds)
{
	QString time = "";
	if (0 > seconds)
	{
		time = "";
	}
	else if (seconds > 3599)
	{
		time = QDateTime::fromTime_t(seconds).toUTC().toString("hh:mm:ss");
	}
	else
	{
		time = QDateTime::fromTime_t(seconds).toUTC().toString("mm:ss");
	}

	return time;
}

void UtilNovatron::CreateTempDirectory()
{
	QString dirTmep = GetTempDirectory();

	QDir dir(dirTmep);
	if (!dir.exists(dirTmep))
	{
		if (dir.mkdir(dirTmep))
		{
//			LogDebug("success to create temp dir...");
		}
	}
}

QString UtilNovatron::GetTempDirectory()
{
	QString dirTemp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	QString organName = QCoreApplication::organizationName();

	return dirTemp + "/" + organName;
}

void UtilNovatron::RemoveTempDirectory()
{
	QString dirTmep = GetTempDirectory();

	QDir dir(dirTmep);
	if (dir.exists(dirTmep))
	{
		dir.removeRecursively();
	}
}

QString UtilNovatron::ConvertURLToFilename(QString fullpath)
{
	QString filename = fullpath;
	int colon = filename.length() - filename.lastIndexOf(":") - 1;
	filename = filename.right(colon);
	colon = filename.length() - filename.indexOf("/") - 1;
	filename = filename.right(colon);
	filename.replace("/", "_");

	QFileInfo fileInfo(filename);
	filename = fileInfo.completeBaseName() + ".jpg";

	return filename;
}

void UtilNovatron::DebugTypeForBrowser(QString title, int type)
{
	if (type & iFolderType_Mask_Play_Top)
	{
		LogDebug("[%s] iFolderType_Mask_Play_Top", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Play_Option)
	{
		LogDebug("[%s] iFolderType_Mask_Play_Option", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Play_Check)
	{
		LogDebug("[%s] iFolderType_Mask_Play_Check", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Play_Select)
	{
		LogDebug("[%s] iFolderType_Mask_Play_Select", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Check)
	{
		LogDebug("[%s] iFolderType_Mask_Check", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_FileMgr)
	{
		LogDebug("[%s] iFolderType_Mask_FileMgr", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_ReadOnly)
	{
		LogDebug("[%s] iFolderType_Mask_ReadOnly", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Sub)
	{
		LogDebug("[%s] iFolderType_Mask_Sub", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Dev)
	{
		LogDebug("[%s] iFolderType_Mask_Dev", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Hdd)
	{
		LogDebug("[%s] iFolderType_Mask_Hdd", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Usb)
	{
		LogDebug("[%s] iFolderType_Mask_Usb", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Cd)
	{
		LogDebug("[%s] iFolderType_Mask_Cd", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Net)
	{
		LogDebug("[%s] iFolderType_Mask_Net", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Upnp)
	{
		LogDebug("[%s] iFolderType_Mask_Upnp", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Root)
	{
		LogDebug("[%s] iFolderType_Mask_Root", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Dir)
	{
		LogDebug("[%s] iFolderType_Mask_Dir", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Song)
	{
		LogDebug("[%s] iFolderType_Mask_Song", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Pls)
	{
		LogDebug("[%s] iFolderType_Mask_Pls", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Cue)
	{
		LogDebug("[%s] iFolderType_Mask_Cue", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_IRadio)
	{
		LogDebug("[%s] iFolderType_Mask_IRadio", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_IsoFile)
	{
		LogDebug("[%s] iFolderType_Mask_IsoFile", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_IsoRoot)
	{
		LogDebug("[%s] iFolderType_Mask_IsoRoot", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Image)
	{
		LogDebug("[%s] iFolderType_Mask_Image", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_File)
	{
		LogDebug("[%s] iFolderType_Mask_File", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Scan)
	{
		LogDebug("[%s] iFolderType_Mask_Scan", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_Media)
	{
		LogDebug("[%s] iFolderType_Mask_Media", title.toUtf8().data());
	}
	if (type & iFolderType_Mask_FilePath)
	{
		LogDebug("[%s] iFolderType_Mask_FilePath", title.toUtf8().data());
	}
}

void UtilNovatron::DebugTypeForIService(QString title, int type)
{
	if (type & iIServiceType_Qobuz)
	{
		LogDebug("[%s] iIServiceType_Qobuz", title.toUtf8().data());
	}
	if (type & iIServiceType_Radios)
	{
		LogDebug("[%s] iIServiceType_Radios", title.toUtf8().data());
	}
	if (type & iIServiceType_Podcasts)
	{
		LogDebug("[%s] iIServiceType_Podcasts", title.toUtf8().data());
	}
	if (type & iIServiceType_Tidal)
	{
		LogDebug("[%s] iIServiceType_Tidal", title.toUtf8().data());
	}
	if (type & iIServiceType_Napster)
	{
		LogDebug("[%s] iIServiceType_Napster", title.toUtf8().data());
	}
	if (type & iIServiceType_Deezer)
	{
		LogDebug("[%s] iIServiceType_Deezer", title.toUtf8().data());
	}
	if (type & iIServiceType_HighResAudio)
	{
		LogDebug("[%s] iIServiceType_HighResAudio", title.toUtf8().data());
	}
	if (type & iIServiceType_AmazonMusic)
	{
		LogDebug("[%s] iIServiceType_AmazonMusic", title.toUtf8().data());
	}
}

void UtilNovatron::DebugTypeForQobuz(QString title, int type)
{
	if (type & iQobuzType_Mask_Artist)
	{
		LogDebug("[%s] iQobuzType_Mask_Artist", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Album)
	{
		LogDebug("[%s] iQobuzType_Mask_Album", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Track)
	{
		LogDebug("[%s] iQobuzType_Mask_Track", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Playlist)
	{
		LogDebug("[%s] iQobuzType_Mask_Playlist", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Search)
	{
		LogDebug("[%s] iQobuzType_Mask_Search", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Recommend)
	{
		LogDebug("[%s] iQobuzType_Mask_Recommend", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Favorite)
	{
		LogDebug("[%s] iQobuzType_Mask_Favorite", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_UserPlaylist)
	{
		LogDebug("[%s] iQobuzType_Mask_UserPlaylist", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Menu_Album)
	{
		LogDebug("[%s] iQobuzType_Mask_Menu_Album", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Menu_Playlist)
	{
		LogDebug("[%s] iQobuzType_Mask_Menu_Playlist", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Menu_Genre)
	{
		LogDebug("[%s] iQobuzType_Mask_Menu_Genre", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Menu_Artist)
	{
		LogDebug("[%s] iQobuzType_Mask_Menu_Artist", title.toUtf8().data());
	}
	if (type & iQobuzType_Mask_Menu_Track)
	{
		LogDebug("[%s] iQobuzType_Mask_Menu_Track", title.toUtf8().data());
	}

}

void UtilNovatron::DebugTypeForQobuzMenu(QString title, int type)
{
	if (type & iQobuzMenu_AddFavorite)
	{
		LogDebug("[%s] iQobuzMenu_AddFavorite", title.toUtf8().data());
	}
	if (type & iQobuzMenu_AddPlaylist)
	{
		LogDebug("[%s] iQobuzMenu_AddPlaylist", title.toUtf8().data());
	}
	if (type & iQobuzMenu_DelFavorite)
	{
		LogDebug("[%s] iQobuzMenu_DelFavorite", title.toUtf8().data());
	}
	if (type & iQobuzMenu_DelPlaylist)
	{
		LogDebug("[%s] iQobuzMenu_DelPlaylist", title.toUtf8().data());
	}
	if (type & iQobuzMenu_DelTrack)
	{
		LogDebug("[%s] iQobuzMenu_DelTrack", title.toUtf8().data());
	}
	if (type & iQobuzMenu_Play)
	{
		LogDebug("[%s] iQobuzMenu_Play", title.toUtf8().data());
	}
	if (type & iQobuzMenu_CoverView)
	{
		LogDebug("[%s] iQobuzMenu_CoverView", title.toUtf8().data());
	}
}

void UtilNovatron::DebugTypeForAirable(QString title, int type)
{
	if (type & iAirableType_Mask_Dir)
	{
		LogDebug("[%s] iAirableType_Mask_Dir", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Sub)
	{
		LogDebug("[%s] iAirableType_Mask_Sub", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Track)
	{
		LogDebug("[%s] iAirableType_Mask_Track", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Program)
	{
		LogDebug("[%s] iAirableType_Mask_Program", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Radio)
	{
		LogDebug("[%s] iAirableType_Mask_Radio", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Feed)
	{
		LogDebug("[%s] iAirableType_Mask_Feed", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Play)
	{
		LogDebug("[%s] iAirableType_Mask_Play", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Art)
	{
		LogDebug("[%s] iAirableType_Mask_Art", title.toUtf8().data());
	}
	if (type & iAirableType_Mask_Logout)
	{
		LogDebug("[%s] iAirableType_Mask_Logout", title.toUtf8().data());
	}

}



