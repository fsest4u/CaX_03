#ifndef CAXCONSTANTS_H
#define CAXCONSTANTS_H

#include <QCoreApplication>

#define		TR(x)	QObject::tr(#x)

//////////////////////////////////////////////////
// Application
//////////////////////////////////////////////////

#define TR_ORGANIZATION				TR("NOVATRON")
#define TR_APPLICATION				TR("Music X Neo")

#define MSG_LIMIT_COUNT				100

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

#define ICON_ITEM_WIDTH				195
#define ICON_ITEM_HEIGHT			220
#define LIST_ITEM_WIDTH				30
#define LIST_ITEM_HEIGHT			30


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
	TOP_MENU_UNSELECT,
	TOP_MENU_GAIN_SET,
	TOP_MENU_GAIN_CLEAR,
	// audio cd
	TOP_MENU_CD_RIPPING,
	TOP_MENU_EJECT_CD,
	// playlist
	TOP_MENU_NEW_PLAYLIST,
	TOP_MENU_ADD_PLAYLIST,
	TOP_MENU_ADD_ITEM,
	TOP_MENU_DELETE_ITEM,
	TOP_MENU_EXPORT_TRACK,
	// browser
	TOP_MENU_OPTION_PLAY_SUBDIR,
	TOP_MENU_CONVERT_FORMAT,
	TOP_MENU_NEW_DIR,
	TOP_MENU_OPTION_OVERWRITE,
	TOP_MENU_MOVE,
	TOP_MENU_COPY,
	TOP_MENU_SEARCH_COVERART,
	TOP_MENU_EDIT_TAG,
	// radio
	TOP_MENU_SEARCH_ALL_N_DELETE,
	TOP_MENU_SEARCH_ALL,
	TOP_MENU_RESERVED_RECORD_LIST,
	TOP_MENU_MAX
};

#define STR_PLAY_NOW				tr("Play now")
#define STR_PLAY_LAST				tr("Play last")
#define STR_PLAY_NEXT				tr("Play next")
#define STR_PLAY_CLEAR				tr("Play clear")
#define STR_RELOAD					tr("Reload")
#define STR_LOAD_COUNT				tr("Load count - %1")
#define STR_SELECT_ALL				tr("Select all")
#define STR_UNSELECT				tr("Unselect")
#define STR_GAIN_SET				tr("Gain Set")
#define STR_GAIN_CLEAR				tr("Gain Clear")
// audio cd
#define STR_CD_RIPPING				tr("CD Ripping")
#define STR_EJECT_CD				tr("Eject CD")
// playlist
#define STR_NEW_PLAYLIST			tr("New playlist")
#define STR_ADD_PLAYLIST			tr("Add playlist")
#define STR_ADD_ITEM				tr("Add item")
#define STR_DELETE_ITEM				tr("Delete item")
#define STR_EXPORT_TRACK			tr("Export track")
// browser
#define STR_OPTION_PLAY_SUBDIR		tr("Play subdirectories - %1")
#define STR_CONVERT_FORMAT    		tr("Conver Audio Format")
#define STR_NEW_DIR           		tr("New Directory")
#define STR_OPTION_OVERWRITE  		tr("Overwrite - %1")
#define STR_MOVE              		tr("Move")
#define STR_COPY              		tr("Copy")
#define STR_SEARCH_COVERART   		tr("Search coverart")
#define STR_EDIT_TAG          		tr("Edit tag")
// radio
#define STR_SEARCH_ALL_N_DELETE		tr("Delete and search all")
#define STR_SEARCH_ALL				tr("Search all")
#define STR_RESERVE_RECORD_LIST		tr("Reserved record list")


#endif // CAXCONSTANTS_H
