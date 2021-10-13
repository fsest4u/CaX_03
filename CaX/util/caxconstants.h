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
// main window
//////////////////////////////////////////////////




#define PLAY_NOW					0
#define PLAY_LAST					1
#define PLAY_NEXT					2
#define PLAY_CLEAR					3

#endif // CAXCONSTANTS_H
