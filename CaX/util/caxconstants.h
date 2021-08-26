#ifndef CAXCONSTANTS_H
#define CAXCONSTANTS_H

#include <QCoreApplication>

#define		TR(x)	QObject::tr(#x)

//////////////////////////////////////////////////
// Application
//////////////////////////////////////////////////

#define TR_ORGANIZATION				TR("NOVATRON")
#define TR_APPLICATION				TR("Music X Neo")

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

#define COVERART_WIDTH				100
#define COVERART_HEIGHT				100
#define CATEGORY_CONT_WIDTH			100
#define CATEGORY_CONT_HEIGHT		100
#define SONG_CONT_WIDTH				70
#define SONG_CONT_HEIGHT			70


//////////////////////////////////////////////////
// main window
//////////////////////////////////////////////////

#define	MAIN_MENU_AUDIO_CD			"AudioCd"
#define	MAIN_MENU_ISERVICE			"IService"
#define	MAIN_MENU_INPUT				"Input"
#define	MAIN_MENU_FM_RADIO			"FmRadio"
#define	MAIN_MENU_GROUP_PLAY		"GroupPlay"
#define	MAIN_MENU_SETUP				"Setup"

#define MAIN_EVENT_ID				"EventID"

#define PLAY_NOW					0
#define PLAY_LAST					1
#define PLAY_NEXT					2
#define PLAY_CLEAR					3

#endif // CAXCONSTANTS_H
