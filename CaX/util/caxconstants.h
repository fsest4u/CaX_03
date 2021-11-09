#ifndef CAXCONSTANTS_H
#define CAXCONSTANTS_H

#include <QCoreApplication>

#define	APP_VERSION					"0.2"
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
#define LIST_ITEM_WIDTH				30
#define LIST_ITEM_HEIGHT			30

#define ICON_HEIGHT_MIN				130
#define ICON_HEIGHT_MAX				160
#define LIST_HEIGHT_MIN				70
#define LIST_HEIGHT_MAx				100

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
	TYPE_MODE_ITEM = 0,
	TYPE_MODE_TRACK,
	TYPE_MODE_ADD_ITEM,
	TYPE_MODE_ADD_TRACK,
	TYPE_MODE_MAX

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
	TOP_MENU_PLAY_NOW = 0,
	TOP_MENU_PLAY_LAST,
	TOP_MENU_PLAY_NEXT,
	TOP_MENU_PLAY_CLEAR,
	TOP_MENU_RELOAD,
	TOP_MENU_LOAD_COUNT,
	TOP_MENU_SELECT_ALL,
	TOP_MENU_CLEAR_ALL,
	TOP_MENU_GAIN_SET,
	TOP_MENU_GAIN_CLEAR,
	TOP_MENU_CD_RIPPING,
	TOP_MENU_EJECT_CD,
	TOP_MENU_ADD_PLAYLIST,
	TOP_MENU_DELETE_PLAYLIST,
	TOP_MENU_ADD_TO_PLAYLIST,
	TOP_MENU_DELETE_TO_PLAYLIST,
	TOP_MENU_ADD_RADIO,
	TOP_MENU_DELETE_RADIO,
	TOP_MENU_RENAME_ITEM,
	TOP_MENU_DELETE_ITEM,
	TOP_MENU_EXPORT_TRACK,
	TOP_MENU_OPTION_PLAY_SUBDIR,
	TOP_MENU_CONVERT_FORMAT,
	TOP_MENU_NEW_DIR,
	TOP_MENU_OPTION_OVERWRITE,
	TOP_MENU_MOVE,
	TOP_MENU_COPY,
	TOP_MENU_SEARCH_COVERART,
	TOP_MENU_EDIT_TAG,
	TOP_MENU_SEARCH_ALL_N_DELETE,
	TOP_MENU_SEARCH_ALL,
	TOP_MENU_RESERVED_RECORD_LIST,
	TOP_MENU_MAX
};

//////////////////////////////////////////////////
// option menu
//////////////////////////////////////////////////

enum
{
	OPTION_MENU_PLAY_NOW = 0,
	OPTION_MENU_PLAY_LAST,
	OPTION_MENU_PLAY_NEXT,
	OPTION_MENU_PLAY_CLEAR,
	OPTION_MENU_ADD_TO_PLAYLIST,
	OPTION_MENU_DELETE_TO_PLAYLIST,
	OPTION_MENU_DELETE_PLAYLIST,
	OPTION_MENU_RENAME_PLAYLIST,
	OPTION_MENU_INFO,
	OPTION_MENU_SEARCH_COVERART,
	OPTION_MENU_GAIN_SET,
	OPTION_MENU_GAIN_CLEAR,
	OPTION_MENU_RENAME_ITEM,
	OPTION_MENU_DELETE_ITEM,
	OPTION_MENU_ADD_TRACK,
	OPTION_MENU_MAX
};


#endif // CAXCONSTANTS_H
