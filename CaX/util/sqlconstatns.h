#ifndef SQLCONSTATNS_H
#define SQLCONSTATNS_H

////////////////////////////////////////////
/// My Music
////////////////////////////////////////////

// 카테고리별 개수 가져오기 --------------------------------------------------------------------------

#define SQL_MY_MUSIC_INFO	"	\
select	\
	(select count(*) from (select * from Song group by song.AlbumID)) as Album	\
	, (select count(*) from (select * from Song group by song.ArtistID)) as Artist	\
	, (select count(*) from (select * from Song group by song.ComposerID)) as Composer	\
	, (select count(*) from (select * from Song group by song.GenreID)) as Genre	\
	, (select count(*) from (select * from Song group by song.MoodID)) as Mood	\
	, (select count(*) from (select * from Song group by song.FolderID)) as Folder	\
	, (select count(*) from (select * from Song group by song.Year)) as Year	\
	, (select count(song.ROWID) from Song) as Song	\
from song	\
limit 1;"

// 카테고리별 목록 가져오기 --------------------------------------------------------------------------

#define SQL_ALBUM_LIST	"	\
select	\
	Album.ROWID as id	\
	, Album.Name as title	\
	, Artist.Name as subtitle	\
	, Album.Favorite as favorite	\
	, Album.Rating as rating	\
	, count(Album.ROWID) as count	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Album.IsDel = 0	\
group by Song.AlbumID	\
order by Song.AlbumID	\
limit 0, 100	\
"
// todo-dylee, temp_code limit 0, 100

#define SQL_ARTIST_LIST	"	\
select	\
	Artist.ROWID as id	\
	, Artist.Name as title	\
	, Artist.Favorite as favorite	\
	, Artist.Rating as rating	\
	, count(Artist.ROWID) as count	\
from Song	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Artist.IsDel = 0	\
group by Song.ArtistID	\
order by Song.ArtistID	\
"

#define SQL_COMPOSER_LIST	"	\
select	\
	Composer.ROWID as id	\
	, Composer.Name as title	\
	, Composer.Favorite as favorite	\
	, Composer.Rating as rating	\
	, count(Composer.ROWID) as count	\
from Song	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
where Composer.IsDel = 0	\
group by Song.ComposerID	\
order by Song.ComposerID	\
"

#define SQL_GENRE_LIST	"	\
select	\
	Genre.ROWID as id	\
	, Genre.Name as title	\
	, Genre.Favorite as favorite	\
	, Genre.Rating as rating	\
	, count(Genre.ROWID) as count	\
from Song	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Genre.IsDel = 0	\
group by Song.GenreID	\
order by Song.GenreID	\
"

#define SQL_MOOD_LIST	"	\
select	\
	Mood.ROWID as id	\
	, Mood.Name as title	\
	, Mood.Favorite as favorite	\
	, Mood.Rating as rating	\
	, count(Mood.ROWID) as count	\
from Song	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Mood.IsDel = 0	\
group by Song.MoodID	\
order by Song.MoodID	\
"

#define SQL_FOLDER_LIST	"	\
select	\
	Folder.ROWID as id	\
	, Folder.Name as title	\
	, Folder.Favorite as favorite	\
	, Folder.Rating as rating	\
	, count(Folder.ROWID) as count	\
from Song	\
inner join Folder on Song.FolderID = Folder.ROWID	\
where Folder.IsDel = 0	\
group by Song.FolderID	\
order by Song.FolderID	\
"

#define SQL_YEAR_LIST	"	\
select	\
	Song.Year as id	\
	, Song.Year as title	\
	, count(*) as count	\
from Song	\
where Song.IsDel = 0	\
group by Song.Year	\
order by Song.Year	\
"


// 카테고리별 하위 목록 가져오기 --------------------------------------------------------------------------

#define SQL_ALBUM_INFO	"	\
select	\
	Album.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1	\
"

#define SQL_SONGS_OF_ALBUM	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1	\
"

#define SQL_ARTIST_INFO	"	\
select	\
	Artist.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.ArtistID = %1	\
"

#define SQL_SONGS_OF_ARTIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.ArtistID = %1	\
"

#define SQL_COMPOSER_INFO	"	\
select	\
	Composer.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.ComposerID = %1	\
"

#define SQL_SONGS_OF_COMPOSER	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.ComposerID = %1	\
"

#define SQL_GENRE_INFO	"	\
select	\
	Genre.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.GenreID = %1	\
"

#define SQL_SONGS_OF_GENRE	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.GenreID = %1	\
"

#define SQL_MOOD_INFO	"	\
select	\
	Mood.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.MoodID = %1	\
"

#define SQL_SONGS_OF_MOOD	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.MoodID = %1	\
"

#define SQL_FOLDER_INFO	"	\
select	\
	Folder.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.FolderID = %1	\
"

#define SQL_SONGS_OF_FOLDER	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.FolderID = %1	\
"

#define SQL_YEAR_INFO	"	\
select	\
	Song.Year as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0 and Song.Year = %1	\
"

#define SQL_SONGS_OF_YEAR	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.Year = %1	\
"

#define SQL_TRACK_INFO	"	\
select	\
	count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Song.IsDel = 0	\
"

#define SQL_SONGS_OF_TRACK	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0	\
"

////////////////////////////////////////////
/// Playlist
////////////////////////////////////////////


#define SQL_PLAYLIST	"	\
select	\
	Pls.ROWID as id	\
	, Pls.Name as title	\
	, count(Pls.rowid) as count	\
from PlsSong	\
inner join Pls on PlsSong.PlsID = Pls.ROWID	\
where Pls.IsDel = 0	\
group by Pls.ROWID	\
order by Pls.Name	\
"

#define SQL_PLAYLIST_INFO	"	\
select	\
	Pls.Name as title	\
	, Artist.Name as Artist	\
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join PlsSong on Song.ROWID = PlsSong.SongID	\
inner join Pls on pls.ROWID = PlsSong.PlsID	\
where PlsSong.PlsID = %1	\
"

#define SQL_SONGS_OF_PLAYLIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_Time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join PlsSong on Song.ROWID = PlsSong.SongID	\
where PlsSong.PlsID = %1	\
order by PlsSong.Seq	\
"

#define SQL_UPDATE_FAVORITE_OF_ALBUM		"update Album set Favorite=%1 where Album.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ALBUMARTIST	"update AlbumArtist set Favorite=%1 where AlbumArtist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ARTIST		"update Artist set Favorite=%1 where Artist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_GENRE		"update Genre set Favorite=%1 where Genre.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_COMPOSER		"update Composer set Favorite=%1 where Composer.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_MOOD			"update Mood set Favorite=%1 where Mood.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_FOLDER		"update Folder set Favorite=%1 where Folder.ROWID=%2"
//#define SQL_UPDATE_FAVORITE_OF_SONG			"update Song set Favorite=%1 where Song.ROWID=%2"

#define SQL_UPDATE_RATING_OF_ALBUM			"update Album set Rating=%1 where Album.ROWID=%2"
#define SQL_UPDATE_RATING_OF_ALBUMARTIST	"update AlbumArtist set Rating=%1 where AlbumArtist.ROWID=%2"
#define SQL_UPDATE_RATING_OF_ARTIST			"update Artist set Rating=%1 where Artist.ROWID=%2"
#define SQL_UPDATE_RATING_OF_GENRE			"update Genre set Rating=%1 where Genre.ROWID=%2"
#define SQL_UPDATE_RATING_OF_COMPOSER		"update Composer set Rating=%1 where Composer.ROWID=%2"
#define SQL_UPDATE_RATING_OF_MOOD			"update Mood set Rating=%1 where Mood.ROWID=%2"
#define SQL_UPDATE_RATING_OF_FOLDER			"update Folder set Rating=%1 where Folder.ROWID=%2"
//#define SQL_UPDATE_RATING_OF_SONG			"update Song set Rating=%1 where Song.ROWID=%2"

#endif // SQLCONSTATNS_H
