#ifndef CAXTRANSLATE_H
#define CAXTRANSLATE_H

#include <QObject>
#include <QIcon>

//////////////////////////////////////////////////
// application
//////////////////////////////////////////////////

#define STR_ORGANIZATION				QObject::tr("NOVATRON")
#define STR_APPLICATION				QObject::tr("Music X Neo")

#define STR_INFO					QObject::tr("Info")
#define STR_WARNING					QObject::tr("")
#define STR_CRITICAL				QObject::tr("")

#define STR_COMING_SOON				QObject::tr("Coming soon")
#define STR_CONFIRM_HERE			QObject::tr("Do you want the selected files to the this folder?")
#define STR_ENTER_SEARCH_WORD		QObject::tr("Please enter a search term of at least 2 characters")
#define STR_INSERT_CD				QObject::tr("Please insert a CD")
#define STR_NO_DEVICES				QObject::tr("No devices found")
#define STR_RESTART_APP				QObject::tr("Change will take effect when you restart Music X Neo.")
#define STR_SELECT_ONE_ITEM			QObject::tr("Please select only one item")
#define STR_SELECT_TARGET_STORAGE	QObject::tr("Select target storage")
#define	STR_UPDATE_FIRMWARE			QObject::tr("Update to the latest firmware (current version: %1)")

//////////////////////////////////////////////////
// dialog
//////////////////////////////////////////////////

#define STR_ADD_TO_DEVICE				QObject::tr("Add to device")
#define STR_ALBUM_INFO					QObject::tr("Album info")
#define STR_CANNOT_DELETE_AUTO_PLAY		QObject::tr("Can't delete Auto Play")
#define STR_CANNOT_RENAME_AUTO_PLAY		QObject::tr("Can't rename Auto Play")
#define STR_DO_YOU_WANT_TO_POWER_OFF	QObject::tr("Do you want to power off?")
#define STR_NEW_PLAYLIST_MADE			QObject::tr("New playlist is made")
#define STR_NO_SELECT					QObject::tr("No select")
#define STR_POWER_OFF_OTHERS			QObject::tr("Power off Others")
#define STR_SELECT_ONLY_ONE_ITEM		QObject::tr("Select only one item")
#define STR_TAG_INFO					QObject::tr("Tag info")
#define STR_TRACK_INFO					QObject::tr("Track info")
#define STR_WOL							QObject::tr("Wake on lan")

//////////////////////////////////////////////////
// response
//////////////////////////////////////////////////

#define STR_INVALID_ID				QObject::tr("invalid command id")
#define STR_INVALID_JSON			QObject::tr("invalid json")
#define STR_UNKNOWN_ERROR			QObject::tr("unknown error")
#define STR_NO_RESULT				QObject::tr("no result")

//////////////////////////////////////////////////
// sidemenu
//////////////////////////////////////////////////

#define STR_MUSIC_DB				QObject::tr("Music DB")
#define STR_AUDIO_CD				QObject::tr("Audio CD")
#define STR_PLAYLIST				QObject::tr("Playlist")
#define STR_BROWSER					QObject::tr("Browser")
#define STR_ISERVICE				QObject::tr("Internet service")
#define STR_INPUT					QObject::tr("Input")
#define STR_FM_RADIO				QObject::tr("FM radio")
#define STR_DAB_RADIO				QObject::tr("DAB radio")
#define STR_GROUP_PLAY				QObject::tr("Group play")
#define STR_SETUP					QObject::tr("Setup")
#define STR_SELECT_DEVICE			QObject::tr("Select device")
#define STR_POWER_ON				QObject::tr("Power on")
#define STR_POWER_OFF				QObject::tr("Power off")
#define STR_LANGUAGE				QObject::tr("Language")
#define STR_ABOUT					QObject::tr("About")
#define STR_SEARCH					QObject::tr("Search")
#define STR_NOW_PLAY				QObject::tr("Now play")

#define STR_AVAILABLE				QObject::tr("Available")
#define STR_NOT_AVAILABLE			QObject::tr("Not available")

//////////////////////////////////////////////////
// menu
//////////////////////////////////////////////////

//#define STR_ADD						QObject::tr("Add")
#define STR_ADD_NEW_STATION			QObject::tr("Add new station")
#define STR_ADD_PLAYLIST			QObject::tr("Add playlist")
#define STR_ADD_SHARE				QObject::tr("Add share")
#define STR_ADD_TO_FAVORITE			QObject::tr("Add to favorite")
#define STR_ADD_TO_PLAYLIST			QObject::tr("Add to playlist")
#define STR_CD_RIPPING				QObject::tr("CD ripping")
#define STR_CHANGE_META_INFO		QObject::tr("Change meta info")
#define STR_CLEAR_ALL				QObject::tr("Clear all")
#define STR_CLEAR_AND_SEEK_ALL		QObject::tr("Clear and seek all")
#define STR_CONVERT_FORMAT    		QObject::tr("Convert audio format")
#define STR_COPY              		QObject::tr("Copy")
#define STR_COPY_HERE              	QObject::tr("Copy here")
#define STR_DELETE					QObject::tr("Delete")
#define STR_DELETE_PLAYLIST			QObject::tr("Delete playlist")
//#define STR_DELETE_RADIO			QObject::tr("Delete radio")
#define STR_DELETE_FROM_PLAYLIST	QObject::tr("Delete track from playlist")
#define STR_DELETE_FROM_PLAY_QUEUE	QObject::tr("Delete from play queue")
#define STR_DELETE_SHARE			QObject::tr("Delete share")
#define STR_DELETE_TO_FAVORITE		QObject::tr("Delete to favorite")
#define STR_EDIT	          		QObject::tr("Edit")
#define STR_EDIT_TAG          		QObject::tr("Edit tag")
#define STR_EJECT_CD				QObject::tr("Eject CD")
#define STR_EXPORT_TRACK			QObject::tr("Export track")
#define STR_FAVORITE				QObject::tr("Favorite")
#define STR_GAIN_SET				QObject::tr("Replaygain")
#define STR_GAIN_CLEAR				QObject::tr("Clear replaygain")
#define STR_GO_TO_ALBUM				QObject::tr("Go to album")
#define STR_GO_TO_ARTIST			QObject::tr("Go to artist")
#define STR_LOAD_COUNT				QObject::tr("Load count")
#define STR_MAKE_FOLDER     		QObject::tr("Make a folder")
#define STR_MAKE_PLAYLIST     		QObject::tr("Make a new playlist")
#define STR_MENU              		QObject::tr("Menu")
#define STR_MODIFY_SHARE			QObject::tr("Modify share")
#define STR_MOVE              		QObject::tr("Move")
#define STR_MOVE_HERE          		QObject::tr("Move here")
#define	STR_NAME					QObject::tr("Name")
#define STR_ON						QObject::tr("on")
#define STR_OFF						QObject::tr("off")
#define STR_OPTION_PLAY_SUBDIR		QObject::tr("Play subdirectories")
#define STR_OPTION_OVERWRITE  		QObject::tr("Overwrite")
#define STR_PLAY_NOW				QObject::tr("Play now")
#define STR_PLAY_LAST				QObject::tr("Play last")
#define STR_PLAY_NEXT				QObject::tr("Play next")
#define STR_PLAY_CLEAR				QObject::tr("Clear && Play")
#define STR_RELOAD					QObject::tr("Reload")
#define STR_REMOVE_FAVORITE			QObject::tr("Remove favorite")
#define STR_RENAME					QObject::tr("Rename")
//#define STR_RENAME_PLAYLIST			QObject::tr("Rename playlist")
#define STR_RESERVE_RECORD_LIST		QObject::tr("Reservation recording list")
#define STR_SCAN_ON					QObject::tr("Scan Music DB")
#define STR_SCAN_OFF				QObject::tr("Remove from Music DB")
#define STR_SEARCH_COVERART			QObject::tr("Search coverart")
#define STR_SEEK_ALL				QObject::tr("Seek all")
#define STR_SELECT_ALL				QObject::tr("Select all")
#define STR_SELECT_COVERART    		QObject::tr("Select coverart")
#define STR_SETUP_RESERVE_RECORD		QObject::tr("Setup reservation recording")
#define STR_SHOW_COLUMNS			QObject::tr("Show columns")
#define STR_TAG_EDIT           		QObject::tr("Tag edit")
#define STR_TITLE           		QObject::tr("Title")

//////////////////////////////////////////////////
// classify menu
//////////////////////////////////////////////////

#define STR_AUDIO_FORMAT			QObject::tr("Audio format")
#define STR_MOST_PLAYED        		QObject::tr("Most played")

#define STR_TOP_10					QObject::tr("Top 10")
#define STR_TOP_25					QObject::tr("Top 25")
#define STR_TOP_50					QObject::tr("Top 50")
#define STR_TOP_100					QObject::tr("Top 100")

//////////////////////////////////////////////////
// sort menu
//////////////////////////////////////////////////

// default sort (category, track)
#define STR_SORT_IMPORTED_DATE		QObject::tr("Sorted by imported date")
#define STR_SORT_ALPHABET			QObject::tr("Sorted by alphabet")
#define STR_SORT_FAVORITE			QObject::tr("Sorted by favorite")
// only category
#define STR_SORT_RATING				QObject::tr("Sorted by rating")

// by category
#define STR_DISP_MODE_TRACK			QObject::tr("Track")
#define STR_DISP_MODE_ALBUM			QObject::tr("Album")
#define STR_DISP_MODE_ARTIST		QObject::tr("Artist")

//////////////////////////////////////////////////
// category
//////////////////////////////////////////////////

#define STR_ALBUM					QObject::tr("Album")
#define STR_ALBUM_ARTIST			QObject::tr("Album Artist")
#define STR_ARTIST					QObject::tr("Artist")
#define STR_COMPOSER				QObject::tr("Composer")
#define STR_GENRE					QObject::tr("Genre")
#define STR_MOOD					QObject::tr("Mood")
#define STR_FOLDER					QObject::tr("Folder")
#define STR_YEAR					QObject::tr("Year")
#define STR_TRACK					QObject::tr("Track")

//////////////////////////////////////////////////
// music db
//////////////////////////////////////////////////

#define STR_ALBUM_GAIN				QObject::tr("Album Gain")
#define STR_BITRATE					QObject::tr("Bitrate")
#define STR_FORMAT					QObject::tr("Format")
#define STR_RATING					QObject::tr("Rating")
#define	STR_SAMPLERATE				QObject::tr("SampleRate")
#define STR_TIME					QObject::tr("Time")
#define STR_TEMPO					QObject::tr("Tempo")

//////////////////////////////////////////////////
// playlist
//////////////////////////////////////////////////

#define STR_AUTO_PLAY					QObject::tr("Auto Play")


//////////////////////////////////////////////////
// browser
//////////////////////////////////////////////////

#define STR_COPY					QObject::tr("Copy")
#define STR_MOVE					QObject::tr("Move")

//////////////////////////////////////////////////
// i-service
//////////////////////////////////////////////////

#define STR_RECOMMEND				QObject::tr("Recommend")
#define STR_FAVORITE				QObject::tr("Favorite")


//////////////////////////////////////////////////
// fm radio
//////////////////////////////////////////////////

#define STR_MY_RADIO				QObject::tr("My radio")

//////////////////////////////////////////////////
// group play
//////////////////////////////////////////////////

#define STR_STEREO				QObject::tr("Stereo")
#define STR_LEFT				QObject::tr("Left")
#define STR_RIGHT				QObject::tr("Right")

//////////////////////////////////////////////////
// search
//////////////////////////////////////////////////

#define STR_SEARCH_RESULTS_FOR			QObject::tr("Search results for \"%1\"")


//////////////////////////////////////////////////
// etc
//////////////////////////////////////////////////



#define STR_UNKNOWN					QObject::tr("Unknown")










#endif // CAXTRANSLATE_H
