#ifndef CAXTRANSLATE_H
#define CAXTRANSLATE_H

#include <QObject>
#include <QIcon>

#define TR(x)	QObject::tr(x)
//////////////////////////////////////////////////
// Application
//////////////////////////////////////////////////

#define TR_ORGANIZATION				TR("NOVATRON")
#define TR_APPLICATION				TR("Music X Neo")

#define STR_INFO					TR("Info")
#define STR_WARNING					TR("Warning")
#define STR_CRITICAL				TR("Critical")

#define STR_COMING_SOON				TR("Coming soon")
#define	STR_UPDATE_FIRMWARE			TR("Update to the latest firmware (current version: %1)")
#define STR_INSERT_CD				TR("Please insert a CD")

//////////////////////////////////////////////////
// response
//////////////////////////////////////////////////

#define STR_INVALID_ID				TR("invalid command id")
#define STR_INVALID_JSON			TR("invalid json")
#define STR_UNKNOWN_ERROR			TR("unknown error")
#define STR_NO_RESULT				TR("no result")

//////////////////////////////////////////////////
// sidemenu
//////////////////////////////////////////////////

#define STR_MUSIC_DB				TR("Music DB")
#define STR_AUDIO_CD				TR("Audio CD")
#define STR_PLAYLIST				TR("Playlist")
#define STR_BROWSER					TR("Browser")
#define STR_ISERVICE				TR("Internet service")
#define STR_INPUT					TR("Input")
#define STR_FM_RADIO				TR("FM radio")
#define STR_DAB_RADIO				TR("DAB radio")
#define STR_GROUP_PLAY				TR("Group play")
#define STR_SETUP					TR("Setup")
#define STR_SELECT_DEVICE			TR("Select device")
#define STR_POWER_ON				TR("Power on")
#define STR_POWER_OFF				TR("Power off")
#define STR_ABOUT					TR("About")

#define STR_AVAILABLE				TR("Available")
#define STR_NOT_AVAILABLE			TR("Not available")

#define STR_MY_RADIO				TR("My radio")
#define STR_UNKNOWN					TR("Unknown")
#define STR_SEARCH					TR("Search")
#define STR_RECOMMEND				TR("Recommend")
#define STR_FAVORITE				TR("Favorite")

#define STR_COPY					TR("Copy")
#define STR_MOVE					TR("Move")


//////////////////////////////////////////////////
// category
//////////////////////////////////////////////////

#define STR_ALBUM					TR("Album")
#define STR_ALBUM_ARTIST			TR("Album Artist")
#define STR_ARTIST					TR("Artist")
#define STR_COMPOSER				TR("Composer")
#define STR_GENRE					TR("Genre")
#define STR_MOOD					TR("Mood")
#define STR_FOLDER					TR("Folder")
#define STR_YEAR					TR("Year")
#define STR_TRACK					TR("Track")


//////////////////////////////////////////////////
// menu
//////////////////////////////////////////////////

#define STR_ADD						TR("Add")
//#define STR_ADD_FAVORITE			TR("Add favorite")
//#define STR_ADD_PLAYLIST			TR("Add playlist")
//#define STR_ADD_RADIO				TR("Add radio")
#define STR_ADD_TO_PLAYLIST			TR("Add to playlist")
#define STR_CD_RIPPING				TR("CD ripping")
#define STR_CLEAR_ALL				TR("Clear all")
#define STR_CONVERT_FORMAT    		TR("Convert audio format")
#define STR_COPY              		TR("Copy")
#define STR_COPY_HERE              	TR("Copy here")
#define STR_DELETE					TR("Delete")
//#define STR_DELETE_FAVORITE			TR("Delete favorite")
//#define STR_DELETE_PLAYLIST			TR("Delete playlist")
//#define STR_DELETE_RADIO			TR("Delete radio")
#define STR_DELETE_TO_PLAYLIST		TR("Delete track to playlist")
#define STR_EDIT_TAG          		TR("Edit tag")
#define STR_EJECT_CD				TR("Eject CD")
#define STR_EXPORT_TRACK			TR("Export track")
#define STR_GAIN_SET				TR("Gain set")
#define STR_GAIN_CLEAR				TR("Gain clear")
#define STR_INFO					TR("Info")
#define STR_LOAD_COUNT				TR("Load count")
#define STR_MOVE              		TR("Move")
#define STR_MOVE_HERE          		TR("Move here")
#define STR_ON						TR("on")
#define STR_OFF						TR("off")
#define STR_OPTION_PLAY_SUBDIR		TR("Play subdirectories")
#define STR_OPTION_OVERWRITE  		TR("Overwrite")
#define STR_PLAY_NOW				TR("Play now")
#define STR_PLAY_LAST				TR("Play last")
#define STR_PLAY_NEXT				TR("Play next")
#define STR_PLAY_CLEAR				TR("Play clear")
#define STR_RELOAD					TR("Reload")
#define STR_RENAME					TR("Rename")
//#define STR_RENAME_PLAYLIST			TR("Rename playlist")
#define STR_RESERVE_RECORD_LIST		TR("Reserved record list")
#define STR_SCAN_ON					TR("Scan on")
#define STR_SCAN_OFF				TR("Scan off")
#define STR_SEARCH_ALL_N_DELETE		TR("Delete and search all")
#define STR_SEARCH_ALL				TR("Search all")
#define STR_SEARCH_COVERART   		TR("Search coverart")
#define STR_SELECT_ALL				TR("Select all")



//////////////////////////////////////////////////
// sort menu
//////////////////////////////////////////////////

#define STR_SORT_NAME				TR("Sorted by Name")
#define STR_SORT_ALBUM				TR("Sorted by Album")
#define STR_SORT_ALBUM_ARTIST		TR("Sorted by Album Artist")
#define STR_SORT_ARTIST				TR("Sorted by Artist")
#define STR_SORT_COMPOSER			TR("Sorted by Composer")
#define STR_SORT_GENRE				TR("Sorted by genre")
#define STR_SORT_MOOD				TR("Sorted by mood")
#define STR_SORT_FOLDER				TR("Sorted by folder")
#define STR_SORT_YEAR				TR("Sorted by year")


//////////////////////////////////////////////////
// dialog
//////////////////////////////////////////////////

#define STR_ASK_POWER_OFF			TR("Do you want to power off?")
#define STR_WOL						TR("Wake on lan")






#endif // CAXTRANSLATE_H
