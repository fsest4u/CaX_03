#ifndef CAXCONSTANTS_H
#define CAXCONSTANTS_H

#include <QCoreApplication>

#define	APP_VERSION					"1.4"
#define FIRMWARE_MIN_VERSION		1720
//////////////////////////////////////////////////
// ssdp
//////////////////////////////////////////////////

#define SSDP_ADDR					"239.255.255.250"
#define SSDP_PORT					1900
#define WOL_PORT					9
//#define SSDP_UDP_TAG				  1000;

#define SSDP_ST_DATA				"urn:schemas-cocktailaudio-com:device:CaWebServer:1"
#define SSDP_CRLF					"\r\n"

#define SSDP_NT						"NT:"
#define SSDP_ST						"ST:"
#define SSDP_NTS					"NTS:"
#define SSDP_MAC_ADDR				"Mac-Addr:"
#define SSDP_CA_NAME				"Ca-Name:"
#define SSDP_CA_DEVICE				"Ca-Device:"
#define SSDP_LOCATION				"LOCATION:"
#define SSDP_NTS_DATA				"ssdp:alive"

//////////////////////////////////////////////////
// device
//////////////////////////////////////////////////

#define DEVICE_ADDR					"Addr"
#define DEVICE_MAC					"Mac"
#define DEVICE_VAL					"Val"
#define DEVICE_DEV					"Dev"
#define DEVICE_APP					"App"
#define DEVICE_VERSION				"Version"
#define DEVICE_WOL_ADDR				"WolAddr"
#define DEVICE_UUID					"UUID"

#define PORT_IMAGE_SERVER			"50006"

//////////////////////////////////////////////////
// ui
//////////////////////////////////////////////////

#define ICON_ITEM_WIDTH				195
#define ICON_ITEM_HEIGHT			220

#define LIST_ITEM_WIDTH				100
#define LIST_BROWSER_HEIGHT			50
#define LIST_DEVICE_HEIGHT			80
#define LIST_SERVICE_HEIGHT			80
#define LIST_TRACKS_HEIGHT			80

#define SEARCH_ITEM_WIDTH			570
#define SEARCH_ITEM_HEIGHT			120

//#define ICON_HEIGHT_MIN				130
#define ICON_HEIGHT_MAX				220
#define ICON_HEIGHT_INIT			160
#define ICON_HEIGHT_MID				105
#define LIST_HEIGHT_MIN				52
//#define LIST_HEIGHT_MAx				100

#define LIST_BROWSER_INDENT_0		5
#define LIST_BROWSER_INDENT_1		45

#define QOBUZ_START					0
#define QOBUZ_COUNT					50

#define SEARCH_WORD_LIMIT_COUNT		3
#define MSG_LIMIT_COUNT				100

#define VOLUME_MIN					0
#define VOLUME_MAX					100

enum
{
	VIEW_MODE_ICON = 0,
	VIEW_MODE_LIST,
	VIEW_MODE_MAX

};

enum
{
	TYPE_MODE_ITEM_TRACK = 0,			// category -> track
	TYPE_MODE_ITEM_ALBUM,				// category -> album
	TYPE_MODE_ITEM_ARTIST,				// genre -> artist
	TYPE_MODE_ITEM_ARTIST_ALBUM,		// genre -> artist -> album
	TYPE_MODE_ITEM_ADD,
	TYPE_MODE_TRACK,
	TYPE_MODE_TRACK_ALBUM,				// category -> album -> track
	TYPE_MODE_TRACK_ALBUM_ARTIST,		// genre -> artist -> album -> track
	TYPE_MODE_TRACK_ADD,
	TYPE_MODE_MAX

};

enum {
	BROWSER_MODE_NORMAL = 0,
	BROWSER_MODE_COPY,
	BROWSER_MODE_COPY_OPTION,
	BROWSER_MODE_MOVE,
	BROWSER_MODE_MOVE_OPTION,
	BROWSER_MODE_MAX
};

//////////////////////////////////////////////////
// play
//////////////////////////////////////////////////

#define PLAY_NONE		-1
#define PLAY_NOW		0
#define PLAY_LAST		1
#define PLAY_NEXT		2
#define PLAY_CLEAR		3

#define RM_NORMAL			"RM_NORMAL"
#define RM_SHUFFLE			"RM_SHUFFLE"
#define RM_RANDOM			"RM_RANDOM"
#define RM_ONCE				"RM_ONCE"
#define RM_R_ONE			"RM_R_ONE"
#define RM_R_ALL			"RM_R_ALL"

#define SRC_MUSIC_DB		"Music DB"
#define SRC_BROWSER			"File"
#define SRC_AUDIO_CD		"Audio CD"

#define SRC_AES_EBU			"AES"
#define SRC_COAXIAL			"COAXIAL"
#define SRC_TOSLINK			"TOSLINK"
#define SRC_ANALOG_IN		"ANALOG IN"
#define SRC_AUX_IN			"AUX IN"
#define SRC_PHONO_IN		"PHONO IN"

#define SRC_FM_RADIO		"FM Radio"
#define SRC_DAB_RADIO		"DAB"
#define SRC_I_RADIO			"i-Radio"
#define SRC_PODCAST			"Podcasts"
#define SRC_TIDAL			"TIDAL"
#define SRC_DEEZER			"Deezer"
#define SRC_NAPSTER			"Napster"
#define SRC_HIGH_RES_AUDIO	"HighResAudio"
#define SRC_AMAZON			"Amazon"
#define SRC_AIRABLE_UPNP	"Airable UPNP"
#define SRC_QOBUZ			"Qobuz"

//////////////////////////////////////////////////
// side menu
//////////////////////////////////////////////////

enum {
	SIDEMENU_MUSIC_DB = Qt::UserRole + 0,
	SIDEMENU_AUDIO_CD,
	SIDEMENU_PLAYLIST,
	SIDEMENU_BROWSER,
	SIDEMENU_ISERVICE,
	SIDEMENU_INPUT,
	SIDEMENU_FM_RADIO,
	SIDEMENU_DAB_RADIO,
	SIDEMENU_GROUP_PLAY,
	SIDEMENU_SETUP,
	SIDEMENU_SELECT_DEVICE,
	SIDEMENU_POWER_ON,
	SIDEMENU_POWER_OFF,
	SIDEMENU_ABOUT,
	SIDEMENU_MAX
};

//////////////////////////////////////////////////
// top menu
//////////////////////////////////////////////////

enum
{
	// common
	TOP_MENU_RELOAD = 0,

	// music db
	TOP_MENU_LOAD_COUNT,

	// common
	TOP_MENU_SELECT_ALL,
	TOP_MENU_CLEAR_ALL,

	// browser
	TOP_MENU_OPTION_PLAY_SUBDIR,

	// common
	TOP_MENU_PLAY_NOW,
	TOP_MENU_PLAY_LAST,
	TOP_MENU_PLAY_NEXT,
	TOP_MENU_PLAY_CLEAR,
	TOP_MENU_GAIN_SET,
	TOP_MENU_GAIN_CLEAR,

	// browser
	TOP_MENU_CONVERT_FORMAT,

	// audio cd
	TOP_MENU_CD_RIPPING,
	TOP_MENU_EJECT_CD,

	// playlist
	TOP_MENU_ADD_TO_PLAYLIST,
	TOP_MENU_DELETE_TO_PLAYLIST,
	TOP_MENU_EXPORT_TRACK,

	// i-service
	TOP_MENU_ADD_TO_FAVORITE,
	TOP_MENU_DELETE_TO_FAVORITE,
	TOP_MENU_ADD_PLAYLIST,
	TOP_MENU_DELETE_PLAYLIST,

	// radio
	TOP_MENU_SEARCH_ALL_N_DELETE,
	TOP_MENU_SEARCH_ALL,
	TOP_MENU_RESERVED_RECORD_LIST,
//	TOP_MENU_ADD_RADIO,
//	TOP_MENU_DELETE_RADIO,


	// browser
	TOP_MENU_ADD,
	TOP_MENU_RENAME,
	TOP_MENU_DELETE,
	TOP_MENU_OPTION_OVERWRITE,
	TOP_MENU_MOVE,
	TOP_MENU_COPY,
	TOP_MENU_MOVE_HERE,
	TOP_MENU_COPY_HERE,
	// common - bottom
//	TOP_MENU_EDIT_TAG, -> move to option menu
	TOP_MENU_SEARCH_COVER_ART,
	TOP_MENU_SHOW_COLUMNS,

	TOP_MENU_MAX
};

//////////////////////////////////////////////////
// option menu
//////////////////////////////////////////////////

enum
{
	// browser
	OPTION_MENU_OPTION_PLAY_SUBDIR = 0,
	// common
	OPTION_MENU_PLAY_NOW,
	OPTION_MENU_PLAY_LAST,
	OPTION_MENU_PLAY_NEXT,
	OPTION_MENU_PLAY_CLEAR,
	OPTION_MENU_GAIN_SET,
	OPTION_MENU_GAIN_CLEAR,

	// browser
	OPTION_MENU_SCAN_ON,
	OPTION_MENU_SCAN_OFF,
	OPTION_MENU_CONVERT_FORMAT,

	// audio cd
	OPTION_MENU_CD_RIPPING,

	// playlist
	OPTION_MENU_ADD_TO_PLAYLIST,
	OPTION_MENU_DELETE_TO_PLAYLIST,
//	OPTION_MENU_RENAME_PLAYLIST,
	OPTION_MENU_DELETE_PLAYLIST,

	// common
	OPTION_MENU_RENAME,
	OPTION_MENU_DELETE,

	// browser
	OPTION_MENU_OPTION_OVERWRITE,
	OPTION_MENU_MOVE,
	OPTION_MENU_COPY,
	OPTION_MENU_MOVE_HERE,
	OPTION_MENU_COPY_HERE,
	OPTION_MENU_EDIT_TAG,
	OPTION_MENU_SEARCH_COVER_ART,

	// i-service
	OPTION_MENU_ADD_FAVORITE,
	OPTION_MENU_DELETE_FAVORITE,

	// search
	OPTION_MENU_GO_TO_ALBUM,

	// common
	OPTION_MENU_TAG_EDIT,

	OPTION_MENU_MAX
};

//////////////////////////////////////////////////
// search cover art
//////////////////////////////////////////////////

#define SEARCH_GOOGLE			"Google"
#define SEARCH_BING				"Bing"
#define SEARCH_AMAZON			"Amazon"
#define SEARCH_GRACENOTE		"Gracenote"

//////////////////////////////////////////////////
// convert format
//////////////////////////////////////////////////

#define FORMAT_CAF				"caf"
#define FORMAT_WAV				"wav"
#define FORMAT_AIFF				"aiff"
#define FORMAT_FLAC				"flac"
#define FORMAT_M4A				"m4a"
#define FORMAT_MP3				"mp3"
#define FORMAT_OGG				"ogg"
#define FORMAT_WMA				"wma"

//////////////////////////////////////////////////
// font (point unit)
//////////////////////////////////////////////////

#define FONT_SIZE_INFO_TITLE			36
#define FONT_SIZE_INFO_SUBTITLE			36
#define FONT_SIZE_INFO_SUBTITLE2		18
#define FONT_SIZE_ICON_TITLE			14
#define FONT_SIZE_ICON_SUBTITLE			14
#define FONT_SIZE_SEARCH_TITLE			20
#define FONT_SIZE_SEARCH_SUBTITLE		16
#define FONT_SIZE_QUEUE_TITLE			26


#define FONT_COLOR_NORMAL		"545454"
#define FONT_COLOR_WHITE		"ffffff"

#define FONT_WEIGHT_NORMAL		"normal"
#define FONT_WEIGHT_BOLD		"bold"



#endif // CAXCONSTANTS_H
