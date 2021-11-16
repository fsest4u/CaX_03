#include <QIcon>

#include "utilnovatron.h"

#include "util/caxtranslate.h"

UtilNovatron::UtilNovatron()
{

}

QIcon UtilNovatron::GetIcon(QString value)
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
//		LogDebug("There is no icon corresponding to the string.");
		return QIcon("");
	}
}
