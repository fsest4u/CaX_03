#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QStandardPaths>
#include <QtNetwork>

#include "utilnovatron.h"

#include "manager/sqlmanager.h"

#include "util/caxconstants.h"
#include "util/caxkeyvalue.h"
#include "util/caxtranslate.h"
#include "util/log.h"

#include "widget/airable.h"
#include "widget/browser.h"
#include "widget/qobuz.h"
#include "widget/setup.h"

UtilNovatron::UtilNovatron()
{

}

QIcon UtilNovatron::GetMenuIcon(QString value)
{
	if (value.contains(STR_MAKE_FOLDER))
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
	else if (value.contains(STR_DELETE_FROM_PLAYLIST))
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
	else if (value.contains(STR_TAG_EDIT))
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
	else if (value.contains(STR_CLEAR_AND_SEEK_ALL))
	{
		return QIcon("");
	}
	else if (value.contains(STR_SEEK_ALL))
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
	else
	{
//		LogDebug("There is no icon corresponding to the string.");
		return QIcon("");
	}
}

QString UtilNovatron::GetSideMenuIcon(QString value)
{
	// side menu
	if (value.contains(STR_MUSIC_DB))
	{
		return (":/resource/submenu-icon40-musicdb.png");
	}
	else if (value.contains(STR_AUDIO_CD))
	{
		return (":/resource/submenu-icon40-audiocd.png");
	}
	else if (value.contains(STR_PLAYLIST))
	{
		return (":/resource/submenu-icon40-playlist.png");
	}
	else if (value.contains(STR_BROWSER))
	{
		return (":/resource/submenu-icon40-browser.png");
	}
	else if (value.contains(STR_ISERVICE))
	{
		return (":/resource/submenu-icon40-internetservice.png");
	}
	else if (value.contains(STR_INPUT))
	{
		return (":/resource/submenu-icon40-input.png");
	}
	else if (value.contains(STR_FM_RADIO))
	{
		return (":/resource/submenu-icon40-fmradio.png");
	}
	else if (value.contains(STR_DAB_RADIO))
	{
		return (":/resource/submenu-icon40-davradio.png");
	}
	else if (value.contains(STR_GROUP_PLAY))
	{
		return (":/resource/submenu-icon40-groupplay.png");
	}
	else if (value.contains(STR_SETUP))
	{
		return (":/resource/submenu-icon40-setup.png");
	}
	else if (value.contains(STR_SELECT_DEVICE))
	{
		return (":/resource/submenu-icon40-selectdevice.png");
	}
	else if (value.contains(STR_POWER_ON))
	{
		return (":/resource/submenu-icon40-wolpoweron.png");
	}
	else if (value.contains(STR_POWER_OFF))
	{
		return (":/resource/submenu-icon40-poweroff.png");
	}
	else if (value.contains(STR_LANGUAGE))
	{
		return (":/resource/submenu-icon40-language.png");
	}
	else if (value.contains(STR_ABOUT))
	{
		return (":/resource/submenu-icon40-aboutmusicx.png");
	}

	return "";
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
		else if (value.contains("DAC"))
		{
			coverArt = ":/resource/input-img160-usbdac-h.png";
		}
		else if (value.contains(SRC_BLUETOOTH))
		{
			coverArt = ":/resource/input-img160-bluetooth-h.png";
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
//		LogDebug("There is no icon corresponding to the string.");
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

QString UtilNovatron::ConvertBitrate(QString bitrate)
{
	int value = bitrate.toInt();
	QString ret = QString::number(value / 1000.0) + "M";
	return ret;
}

QString UtilNovatron::ConvertSamplerate(QString samplerate)
{
	int value = samplerate.toInt();
	QString ret = QString::number(value / 1000.0) + "K";
	return ret;
}

QString UtilNovatron::GetTempDirectory()
{
	QString dirTemp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	QString organName = QCoreApplication::organizationName();

	return dirTemp + "/" + organName;
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

void UtilNovatron::RemoveTempDirectory()
{
	QString dirTmep = GetTempDirectory();

	QDir dir(dirTmep);
	if (dir.exists(dirTmep))
	{
		dir.removeRecursively();
	}
}

void UtilNovatron::RemoveFilesInTempDirectory(QString prefix)
{
	QString dirTmep = GetTempDirectory();
	QDir dir(dirTmep, prefix + "*");
	for(const QString & filename: dir.entryList())
	{
//		LogDebug("filename [%s]", filename.toUtf8().data());
		dir.remove(filename);
	}
}


void UtilNovatron::RemoveContainFilesInTempDirectory(QString part)
{
	QString dirTmep = GetTempDirectory();
	QDir dir(dirTmep);
	for(const QString & filename: dir.entryList())
	{
		if (filename.contains(part))
		{
//			LogDebug("filename [%s]", filename.toUtf8().data());
			dir.remove(filename);
		}
	}
}

QString UtilNovatron::ConvertURLToFilenameWithExtension(QString fullpath)
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

QString UtilNovatron::ConvertURLToFilename(QString fullpath)
{
	QString filename = fullpath;
	int colon = filename.length() - filename.lastIndexOf(":") - 1;
	filename = filename.right(colon);
	colon = filename.length() - filename.indexOf("/") - 1;
	filename = filename.right(colon);
	filename.replace("/", "_");

	return filename;
}

QString UtilNovatron::GetCategoryName(int category)
{
	QString title;

	switch(category)
	{
	case SQLManager::CATEGORY_ALBUM:
		title = KEY_ALBUM;
		break;
	case SQLManager::CATEGORY_ALBUM_ARTIST:
		title = KEY_ALBUM_ARTIST;
		break;
	case SQLManager::CATEGORY_ARTIST:
		title = KEY_ARTIST;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		title = KEY_COMPOSER;
		break;
	case SQLManager::CATEGORY_GENRE:
		title = KEY_GENRE;
		break;
	case SQLManager::CATEGORY_MOOD:
		title = KEY_MOOD;
		break;
	case SQLManager::CATEGORY_FOLDER:
		title = KEY_FOLDER;
		break;
	case SQLManager::CATEGORY_YEAR:
		title = KEY_YEAR;
		break;
	default:
		title = KEY_SONG;
		break;
	}

	return title;
}

QString UtilNovatron::GetCategoryTitleName(int category)
{
	QString title = KEY_ALBUM;
	switch (category)
	{
	case SQLManager::CATEGORY_ALBUM:
		title = KEY_ALBUM;
		break;
	case SQLManager::CATEGORY_ALBUM_ARTIST:
		title = KEY_ALBUM_ARTIST;
		break;
	case SQLManager::CATEGORY_ARTIST:
		title = KEY_ARTIST;
		break;
	case SQLManager::CATEGORY_GENRE:
		title = KEY_GENRE;
		break;
	case SQLManager::CATEGORY_COMPOSER:
		title = KEY_COMPOSER;
		break;
	case SQLManager::CATEGORY_MOOD:
		title = KEY_MOOD;
		break;
	case SQLManager::CATEGORY_FOLDER:
		title = KEY_FOLDER;
		break;
	case SQLManager::CATEGORY_YEAR:
		title = KEY_YEAR;
		break;
	case SQLManager::CATEGORY_TRACK:
		title = KEY_TRACK;
		break;
	}

	return title;

}

int UtilNovatron::GetCategoryCount(int category, CJsonNode node)
{
	int totalCount = 0;

	switch (category)
	{
	case SQLManager::CATEGORY_ALBUM:
		totalCount = node.GetString(KEY_ALBUM).toInt();
		break;
	case SQLManager::CATEGORY_ALBUM_ARTIST:
		totalCount = node.GetString(KEY_ALBUM_ARTIST).toInt();
		break;
	case SQLManager::CATEGORY_ARTIST:
		totalCount = node.GetString(KEY_ARTIST).toInt();
		break;
	case SQLManager::CATEGORY_COMPOSER:
		totalCount = node.GetString(KEY_COMPOSER).toInt();
		break;
	case SQLManager::CATEGORY_GENRE:
		totalCount = node.GetString(KEY_GENRE).toInt();
		break;
	case SQLManager::CATEGORY_MOOD:
		totalCount = node.GetString(KEY_MOOD).toInt();
		break;
	case SQLManager::CATEGORY_FOLDER:
		totalCount = node.GetString(KEY_FOLDER).toInt();
		break;
	case SQLManager::CATEGORY_YEAR:
		totalCount = node.GetString(KEY_YEAR).toInt();
		break;
	case SQLManager::CATEGORY_TRACK:
		totalCount = node.GetString(KEY_SONG).toInt();
		break;
	default:
		break;
	}

	return totalCount;
}

QString UtilNovatron::GetSuffix(QString filename)
{
	int index = filename.length() - filename.lastIndexOf('.') - 1;
	return filename.right(index);
}

QString UtilNovatron::GetTitleForSetup(QString name)
{
	QString title;

	// setup main
	if (!name.compare("Music DB"))
	{
		title = STR_SETUP_MUSIC_DB;
	}
	else if (!name.compare("CD Ripping"))
	{
		title = STR_SETUP_CD_RIPPING;
	}
	else if (!name.compare("Audio"))
	{
		title = STR_SETUP_AUDIO;
	}
	else if (!name.compare("Network"))
	{
		title = STR_SETUP_NETWORK;
	}
	else if (!name.compare("Network Service"))
	{
		title = STR_SETUP_NETWORK_SERVICE;
	}
	else if (!name.compare("Internet Service"))
	{
		title = STR_SETUP_INTERNET_SERVICE;
	}
	else if (!name.compare("System"))
	{
		title = STR_SETUP_SYSTEM;
	}
	else if (!name.compare("Time"))
	{
		title = STR_SETUP_TIME;
	}
	else if (!name.compare("Firmware"))
	{
		title = STR_SETUP_FIRMWARE;
	}
	else if (!name.compare("License"))
	{
		title = STR_SETUP_LICENSE;
	}
	// setup music db
	else if (!name.compare("Storage"))
	{
		title = STR_SETUP_STORAGE;
	}
	else if (!name.compare("Music DB Scan Status"))
	{
		title = STR_SETUP_MUSIC_DB_SCAN_STATUS;
	}
	else if (!name.compare("Rescan Resource"))
	{
		title = STR_SETUP_RESCAN_RESOURCE;
	}
	else if (!name.compare("Music DB Initialization"))
	{
		title = STR_SETUP_MUSIC_DB_INITIALIZE;
	}
	// setup cd ripping
	else if (!name.compare("Bit Rate"))
	{
		title = STR_SETUP_BIT_RATE;
	}
	else if (!name.compare("Quality"))
	{
		title = STR_SETUP_QUALITY;
	}
	else if (!name.compare("Audio Rip Format"))
	{
		title = STR_SETUP_AUDIO_RIP_FORMAT;
	}
	else if (!name.compare("Auto Rip"))
	{
		title = STR_SETUP_AUTO_RIP;
	}
	// setup audio
	else if (!name.compare("Max Volume"))
	{
		title = STR_SETUP_MAX_VOLUME;
	}
	else if (!name.compare("Equalizer"))
	{
		title = STR_SETUP_EQUALIZER;
	}
	else if (!name.compare("USB/HDMI Audio Out"))
	{
		title = STR_SETUP_USB_HDMI_AUDIO_OUT;
	}
	else if (!name.compare("DSD Signal Type"))
	{
		title = STR_SETUP_DSD_SIGNAL_TYPE;
	}
	else if (!name.compare("Analog In Volume"))
	{
		title = STR_SETUP_ANALOG_IN_VOLUME;
	}
	else if (!name.compare("Analog Out Volume"))
	{
		title = STR_SETUP_ANALOG_OUT_VOLUME;
	}
	else if (!name.compare("Analog Out"))
	{
		title = STR_SETUP_ANALOG_OUT;
	}
	else if (!name.compare("Recording Sample Rate"))
	{
		title = STR_SETUP_RECORDING_SAMPLE_RATE;
	}
	else if (!name.compare("Digital Out Volume"))
	{
		title = STR_SETUP_DIGITAL_OUT_VOLUME;
	}
	else if (!name.compare("Digital Out"))
	{
		title = STR_SETUP_DIGITAL_OUT;
	}
	else if (!name.compare("AES/EBU"))
	{
		title = STR_SETUP_AES_EBU;
	}
	else if (!name.compare("SPDIF"))
	{
		title = STR_SETUP_SPDIF;
	}
	else if (!name.compare("Speaker Out"))
	{
		title = STR_SETUP_SPEAKER_OUT;
	}
	else if (!name.compare("Gapless Play"))
	{
		title = STR_SETUP_GAPLESS_PLAY;
	}
	else if (!name.compare("Replaygain"))
	{
		title = STR_SETUP_REPLAYGAIN;
	}
	else if (!name.compare("Limit Sample Rate in Audio Convert"))
	{
		title = STR_SETUP_LIMIT_SAMPLE_RATE;
	}
	else if (!name.compare("Power On Volume"))
	{
		title = STR_SETUP_POWER_ON_VOLUME;
	}
	// setup network
	else if (!name.compare("Network"))
	{
		title = STR_SETUP_NETWORK;
	}
	else if (!name.compare("Wired Setup"))
	{
		title = STR_SETUP_WIRED_SETUP;
	}
	else if (!name.compare("Wireless Setup"))
	{
		title = STR_SETUP_WIRELESS_SETUP;
	}
	else if (!name.compare("Network Info"))
	{
		title = STR_SETUP_NETWORK_INFO;
	}
	else if (!name.compare("Wireless Info"))
	{
		title = STR_SETUP_WIRELESS_INFO;
	}
	else if (!name.compare("Wake On LAN"))
	{
		title = STR_SETUP_WAKE_ON_LAN;
	}
	// setup network service
	else if (name.contains("Samba Server"))
	{
		title = STR_SETUP_SAMBA_SERVER;
	}
	else if (!name.compare("Host Name"))
	{
		title = STR_SETUP_HOST_NAME;
	}
	else if (!name.compare("Workgroup"))
	{
		title = STR_SETUP_WORKGROUP;
	}
	else if (name.contains("UPnP Server"))
	{
		title = STR_SETUP_UPNP_SERVER;
	}
	else if (name.contains("Ftp Server"))
	{
		title = STR_SETUP_FTP_SERVER;
	}
	else if (!name.compare("Password"))
	{
		title = STR_SETUP_PASSWORD;
	}
	else if (name.contains("Shareplay(Airplay)"))
	{
		title = STR_SETUP_SHARE_PLAY;
	}
	// setup internet service
	else if (!name.compare("Qobuz Service"))
	{
		title = STR_SETUP_QOBUZ_SERVICE;
	}
	else if (!name.compare("TIDAL Service"))
	{
		title = STR_SETUP_TIDAL_SERVICE;
	}
	else if (!name.compare("Deezer Service"))
	{
		title = STR_SETUP_DEEZER_SERVICE;
	}
	else if (!name.compare("Napster Service"))
	{
		title = STR_SETUP_NAPSTER_SERVICE;
	}
	else if (!name.compare("HighResAudio Service"))
	{
		title = STR_SETUP_HIGHRESAUDIO_SERVICE;
	}
	else if (!name.compare("Amazon Music"))
	{
		title = STR_SETUP_AMAZON_MUSIC;
	}
	else if (!name.compare("User Name"))
	{
		title = STR_SETUP_USER_NAME;
	}
	else if (!name.compare("Audio Format"))
	{
		title = STR_SETUP_AUDIO_FORMAT;
	}
	else if (!name.compare("Stream Quality"))
	{
		title = STR_SETUP_STREAM_QUALITY;
	}
	// setup system
	else if (!name.compare("OSD Language"))
	{
		title = STR_SETUP_OSD_LANGUAGE;
	}
	else if (!name.compare("FM broadcast band"))
	{
		title = STR_SETUP_FM_BROADCAST_BAND;
	}
	else if (!name.compare("Use RDS When Seek All"))
	{
		title = STR_SETUP_USE_RDS_WHEN_SEEK_ALL;
	}
	else if (!name.compare("Auto Play"))
	{
		title = STR_SETUP_AUTO_PLAY;
	}
	else if (!name.compare("Resume Play"))
	{
		title = STR_SETUP_RESUME_PLAY;
	}
	else if (!name.compare("Factory Reset"))
	{
		title = STR_SETUP_FACTORY_RESET;
	}
	else if (!name.compare("HDD Sleep"))
	{
		title = STR_SETUP_HDD_SLEEP;
	}
	else if (!name.compare("Format Storage"))
	{
		title = STR_SETUP_FORMAT_STORAGE;
	}
	else if (!name.compare("File System Fix"))
	{
		title = STR_SETUP_FILE_SYSTEM_FIX;
	}
	else if (!name.compare("Alarm"))
	{
		title = STR_SETUP_ALARM;
	}
	else if (!name.compare("Auto Shutdown"))
	{
		title = STR_SETUP_AUTO_SHUTDOWN;
	}
	else if (!name.compare("ERP Regulation"))
	{
		title = STR_SETUP_ERP_REGULATION;
	}
	// setup time
	else if (!name.compare("Time Format"))
	{
		title = STR_SETUP_TIME_FORMAT;
	}
	else if (!name.compare("Time Set (Internet)"))
	{
		title = STR_SETUP_TIME_SET_INTERNET;
	}
	else if (!name.compare("Time Set (Manually)"))
	{
		title = STR_SETUP_TIME_SET_MANUALLY;
	}
	// setup firmware
	else if (!name.compare("Update manually"))
	{
		title = STR_SETUP_UPDATE_MANUALLY;
	}
	else if (!name.compare("Update automatically"))
	{
		title = STR_SETUP_UPDATE_AUTOMATICALLY;
	}
	else if (!name.compare("Check now for new firmware"))
	{
		title = STR_SETUP_CHECK_NEW_FIRMWARE;
	}
	// setup license
	else if (!name.compare("Gracenote(Cd)"))
	{
		title = STR_SETUP_GRACENOTE_CD;
	}
	else if (!name.compare("Gracenote(Cover)"))
	{
		title = STR_SETUP_GRACENOTE_COVER;
	}
	else if (!name.compare("Gracenote(Finger)"))
	{
		title = STR_SETUP_GRACENOTE_FINGER;
	}
	else if (!name.compare("Gracenote(Pls)"))
	{
		title = STR_SETUP_GRACENOTE_PLS;
	}
	else
	{
//		LogDebug("There is no title corresponding to the string.");
		title = name;
	}

//	LogDebug("name [%s] title [%s]", name.toUtf8().data(), title.toUtf8().data());
	return title;
}

Loading *UtilNovatron::LoadingStart(QWidget *widget)
{
	if (widget != nullptr)
	{
		Loading *loading = new Loading(widget);
		loading->Start();

		return loading;
	}

	return nullptr;
}

void UtilNovatron::LoadingStop(Loading *loading)
{
	if (loading != nullptr)
	{
		loading->Stop();
	}
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
	if (type & iAirableType_Mask_Record)
	{
		LogDebug("[%s] iAirableType_Mask_Record", title.toUtf8().data());
	}

}

void UtilNovatron::DebugTypeForSetup(QString title, int type)
{
	if (type & iAppSetupType_Mask_Enum)
	{
		LogDebug("[%s] iAppSetupType_Mask_Enum", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Select)
	{
		LogDebug("[%s] iAppSetupType_Mask_Select", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_List)
	{
		LogDebug("[%s] iAppSetupType_Mask_List", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Exec)
	{
		LogDebug("[%s] iAppSetupType_Mask_Exec", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_FormSelect)
	{
		LogDebug("[%s] iAppSetupType_Mask_FormSelect", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_App)
	{
		LogDebug("[%s] iAppSetupType_Mask_App", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Sub)
	{
		LogDebug("[%s] iAppSetupType_Mask_Sub", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_PlayDisable)
	{
		LogDebug("[%s] iAppSetupType_Mask_PlayDisable", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Disable)
	{
		LogDebug("[%s] iAppSetupType_Mask_Disable", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_HideValue)
	{
		LogDebug("[%s] iAppSetupType_Mask_HideValue", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Browser)
	{
		LogDebug("[%s] iAppSetupType_Mask_Browser", title.toUtf8().data());
	}
	if (type & iAppSetupType_Mask_Event)
	{
		LogDebug("[%s] iAppSetupType_Mask_Event", title.toUtf8().data());
	}
}

void UtilNovatron::DebugTypeForSetupBtn(QString title, int type)
{
	if (type == iSetupBtnAction_None)
	{
		LogDebug("[%s] iSetupBtnAction_None", title.toUtf8().data());
	}
	if (type == iSetupBtnAction_Request)
	{
		LogDebug("[%s] iSetupBtnAction_Request", title.toUtf8().data());
	}
	if (type == iSetupBtnAction_Event)
	{
		LogDebug("[%s] iSetupBtnAction_Event", title.toUtf8().data());
	}
	if (type == iSetupBtnAction_OpenWeb)
	{
		LogDebug("[%s] iSetupBtnAction_OpenWeb", title.toUtf8().data());
	}
	if (type == iSetupBtnAction_Cancel)
	{
		LogDebug("[%s] iSetupBtnAction_Cancel", title.toUtf8().data());
	}
	if (type == iSetupBtnAction_Max)
	{
		//LogDebug("[%s] iSetupBtnAction_Max", title.toUtf8().data());
	}
}

void UtilNovatron::DebugTypeForSetupInput(QString title, int type)
{
	if (type == iSetupInput_Disp)
	{
		LogDebug("[%s] iSetupInput_Disp", title.toUtf8().data());
	}
	if (type == iSetupInput_Label)
	{
		LogDebug("[%s] iSetupInput_Label", title.toUtf8().data());
	}
	if (type == iSetupInput_Hidden)
	{
		LogDebug("[%s] iSetupInput_Hidden", title.toUtf8().data());
	}
	if (type == iSetupInput_Slider)
	{
		LogDebug("[%s] iSetupInput_Slider", title.toUtf8().data());
	}
	if (type == iSetupInput_List)
	{
		LogDebug("[%s] iSetupInput_List", title.toUtf8().data());
	}
	if (type == iSetupInput_IP)
	{
		LogDebug("[%s] iSetupInput_IP", title.toUtf8().data());
	}
	if (type == iSetupInput_Password)
	{
		LogDebug("[%s] iSetupInput_Password", title.toUtf8().data());
	}
	if (type == iSetupInput_Text)
	{
		LogDebug("[%s] iSetupInput_Text", title.toUtf8().data());
	}
	if (type == iSetupInput_Time)
	{
		LogDebug("[%s] iSetupInput_Time", title.toUtf8().data());
	}
	if (type == iSetupInput_Switch)
	{
		LogDebug("[%s] iSetupInput_Switch", title.toUtf8().data());
	}
	if (type == iSetupInput_CurrDate)
	{
		LogDebug("[%s] iSetupInput_CurrDate", title.toUtf8().data());
	}
	if (type == iSetupInput_CurrTime)
	{
		LogDebug("[%s] iSetupInput_CurrTime", title.toUtf8().data());
	}

}

void UtilNovatron::DebugTypeForUDP(QString title, int flag)
{
	if (flag & QNetworkInterface::IsUp)
	{
		LogDebug("[%s] IsUp", title.toUtf8().data());
	}
	if (flag & QNetworkInterface::IsRunning)
	{
		LogDebug("[%s] IsRunning", title.toUtf8().data());
	}
	if (flag & QNetworkInterface::CanBroadcast)
	{
		LogDebug("[%s] CanBroadcast", title.toUtf8().data());
	}
	if (flag & QNetworkInterface::IsLoopBack)
	{
		LogDebug("[%s] IsLoopBack", title.toUtf8().data());
	}
	if (flag & QNetworkInterface::IsPointToPoint)
	{
		LogDebug("[%s] IsPointToPoint", title.toUtf8().data());
	}
	if (flag & QNetworkInterface::CanMulticast)
	{
		LogDebug("[%s] CanMulticast", title.toUtf8().data());
	}
}



