#ifndef SQLCONSTATNS_H
#define SQLCONSTATNS_H

////////////////////////////////////////////
/// My Music
////////////////////////////////////////////

// 카테고리별 개수 가져오기 --------------------------------------------------------------------------

#define SQL_MUSIC_DB_OVERVIEW	"	\
select	\
	(select count(*) from (select * from Song where Song.IsDel = 0 group by song.AlbumID)) as Album	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.ArtistID)) as Artist	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.ComposerID)) as Composer	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.GenreID)) as Genre	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.MoodID)) as Mood	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.FolderID)) as Folder	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.Year)) as Year	\
	, (select count(song.ROWID) from Song) as Song	\
from song	\
limit 1;"

// 카테고리별 목록 가져오기 --------------------------------------------------------------------------

#define SQL_ALBUM_LIST	"	\
select	\
	Album.ROWID as id	\
	, Album.Name as title	\
	, Artist.Name as subtitle	\
	, Album.favorite as favorite	\
	, Album.rating as rating	\
	, count(Album.ROWID) as count	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Album.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.AlbumID	\
order by Song.%6 %7	\
limit %8, %9"

#define SQL_ARTIST_LIST	"	\
select	\
	Artist.ROWID as id	\
	, Artist.Name as title	\
	, Artist.favorite as favorite	\
	, Artist.rating as rating	\
	, count(Artist.ROWID) as count	\
from Song	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Artist.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.ArtistID	\
order by Song.%6 %7	\
limit %8, %9"


#define SQL_COMPOSER_LIST	"	\
select	\
	Composer.ROWID as id	\
	, Composer.Name as title	\
	, Composer.favorite as favorite	\
	, Composer.rating as rating	\
	, count(Composer.ROWID) as count	\
from Song	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
where Composer.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.ComposerID	\
order by Song.%6 %7	\
limit %8, %9"

#define SQL_GENRE_LIST	"	\
select	\
	Genre.ROWID as id	\
	, Genre.Name as title	\
	, Genre.favorite as favorite	\
	, Genre.rating as rating	\
	, count(Genre.ROWID) as count	\
from Song	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Genre.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.GenreID	\
order by Song.%6 %7	\
limit %8, %9"

#define SQL_MOOD_LIST	"	\
select	\
	Mood.ROWID as id	\
	, Mood.Name as title	\
	, Mood.favorite as favorite	\
	, Mood.rating as rating	\
	, count(Mood.ROWID) as count	\
from Song	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Mood.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.MoodID	\
order by Song.%6 %7	\
limit %8, %9"

#define SQL_FOLDER_LIST	"	\
select	\
	Folder.ROWID as id	\
	, Folder.Name as title	\
	, Folder.favorite as favorite	\
	, Folder.rating as rating	\
	, count(Folder.ROWID) as count	\
from Song	\
inner join Folder on Song.FolderID = Folder.ROWID	\
where Folder.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.FolderID	\
order by Song.%6 %7	\
limit %8, %9"

#define SQL_YEAR_LIST	"	\
select	\
	Song.Year as id	\
	, Song.Year as title	\
	, count(*) as count	\
from Song	\
where Song.IsDel = 0	%1 %2 %3 %4 %5 \
group by Song.Year	\
order by Song.%6 %7	\
limit %8, %9"


// 카테고리별 하위 목록 가져오기 --------------------------------------------------------------------------

#define SQL_ALBUM_OVERVIEW	"	\
select	\
	Album.Name as title	\
	, Artist.Name as Artist	\
	, Album.favorite as favorite \
	, Album.rating as rating \
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

#define SQL_ALBUM_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_ARTIST_OVERVIEW	"	\
select	\
	Artist.Name as title	\
	, Artist.Name as Artist	\
	, Artist.favorite as Favorite \
	, Artist.rating as rating \
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

#define SQL_ARTIST_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.ArtistID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_COMPOSER_OVERVIEW	"	\
select	\
	Composer.Name as title	\
	, Artist.Name as Artist	\
	, Composer.favorite as favorite \
	, Composer.rating as rating \
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
where Song.IsDel = 0 and Song.ComposerID = %1	\
"

#define SQL_COMPOSER_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.ComposerID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_GENRE_OVERVIEW	"	\
select	\
	Genre.Name as title	\
	, Artist.Name as Artist	\
	, Genre.favorite as favorite \
	, Genre.rating as rating \
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.GenreID = %1	\
"

#define SQL_GENRE_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.GenreID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_MOOD_OVERVIEW	"	\
select	\
	Mood.Name as title	\
	, Artist.Name as Artist	\
	, Mood.favorite as favorite \
	, Mood.rating as rating \
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 and Song.MoodID = %1	\
"

#define SQL_MOOD_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.MoodID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_FOLDER_OVERVIEW	"	\
select	\
	Folder.Name as title	\
	, Artist.Name as Artist	\
	, Folder.favorite as favorite \
	, Folder.rating as rating \
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Folder on Song.FolderID = Folder.ROWID	\
where Song.IsDel = 0 and Song.FolderID = %1	\
"

#define SQL_FOLDER_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.FolderID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_YEAR_OVERVIEW	"	\
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

#define SQL_YEAR_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 and Song.Year = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_TRACK_OVERVIEW	"	\
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

#define SQL_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, Song.favorite as favorite	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Song.IsDel = 0 \
order by Song.%1 %2	\
limit %3, %4"

#define SQL_UPDATE_FAVORITE_OF_ALBUM		"update Album set Favorite=%1 where Album.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ALBUMARTIST	"update AlbumArtist set Favorite=%1 where AlbumArtist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ARTIST		"update Artist set Favorite=%1 where Artist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_GENRE		"update Genre set Favorite=%1 where Genre.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_COMPOSER		"update Composer set Favorite=%1 where Composer.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_MOOD			"update Mood set Favorite=%1 where Mood.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_FOLDER		"update Folder set Favorite=%1 where Folder.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_SONG			"update Song set Favorite=%1 where Song.ROWID=%2"

#define SQL_UPDATE_RATING_OF_ALBUM			"update Album set Rating=%1 where Album.ROWID=%2"
#define SQL_UPDATE_RATING_OF_ALBUMARTIST	"update AlbumArtist set Rating=%1 where AlbumArtist.ROWID=%2"
#define SQL_UPDATE_RATING_OF_ARTIST			"update Artist set Rating=%1 where Artist.ROWID=%2"
#define SQL_UPDATE_RATING_OF_GENRE			"update Genre set Rating=%1 where Genre.ROWID=%2"
#define SQL_UPDATE_RATING_OF_COMPOSER		"update Composer set Rating=%1 where Composer.ROWID=%2"
#define SQL_UPDATE_RATING_OF_MOOD			"update Mood set Rating=%1 where Mood.ROWID=%2"
#define SQL_UPDATE_RATING_OF_FOLDER			"update Folder set Rating=%1 where Folder.ROWID=%2"
//#define SQL_UPDATE_RATING_OF_SONG			"update Song set Rating=%1 where Song.ROWID=%2"


#define SQL_CLASSIFY			"\
SELECT \
	Song.%1ID AS id, %1.Name AS name, count(*) AS cnt \
FROM Song \
INNER JOIN %1 on Song.%1ID = %1.ROWID \
GROUP BY Song.%1ID \
ORDER BY cnt DESC LIMIT 0, 20\
"


////////////////////////////////////////////
/// Playlist
////////////////////////////////////////////


#define SQL_PLAYLIST	"	\
select	\
	Pls.ROWID as id	\
	, Pls.Name as title	\
	, count(Pls.ROWID) as count	\
from PlsSong	\
inner join Pls on Pls.ROWID = PlsSong.PlsID	\
where Pls.IsDel = 0	\
group by Pls.ROWID	\
order by Pls.Name	\
"

#define SQL_PLAYLIST_OVERVIEW	"	\
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
inner join Pls on Pls.ROWID = PlsSong.PlsID	\
where PlsSong.PlsID = %1	\
"

#define SQL_PLAYLIST_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.nov_time as time	\
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

////////////////////////////////////////////
/// Category list
////////////////////////////////////////////

#define SQL_CATEGORY_ALBUM_LIST	"	\
select	\
	Album.Name as Album	\
from Album	\
where Album.IsDel = 0	\
"

#define SQL_CATEGORY_ALBUM_ARTIST_LIST	"	\
select	\
	AlbumArtist.Name as AlbumArtist	\
from AlbumArtist	\
where AlbumArtist.IsDel = 0	\
"

#define SQL_CATEGORY_ARTIST_LIST	"	\
select	\
	Artist.Name as Artist	\
from Artist	\
where Artist.IsDel = 0	\
"

#define SQL_CATEGORY_GENRE_LIST	"	\
select	\
	Genre.Name as Genre	\
from Genre	\
where Genre.IsDel = 0	\
"

#define SQL_CATEGORY_COMPOSER_LIST	"	\
select	\
	Composer.Name as Composer	\
from Composer	\
where Composer.IsDel = 0	\
"

#define SQL_CATEGORY_MOOD_LIST	"	\
select	\
	Mood.Name as Mood	\
from Mood	\
where Mood.IsDel = 0	\
"

////////////////////////////////////////////
/// Search result list
////////////////////////////////////////////

#define SQL_SEARCH_ALBUM_LIST	"	\
SELECT	\
	Album.ROWID as id,	\
	Album.Name as title,	\
	Artist.Name as subtitle	\
FROM Song	\
INNER JOIN Album on Song.AlbumID = Album.ROWID	\
INNER JOIN Artist on Song.ArtistID = Artist.ROWID	\
WHERE Album.IsDel = 0 AND Album.Name like \"%%1%\"	\
GROUP BY Song.AlbumID	\
ORDER BY Album.Name	\
LIMIT 0, 100	\
"

#define SQL_SEARCH_ARTIST_LIST	"	\
SELECT	\
	Artist.ROWID as id,	\
	Artist.Name as title	\
FROM Song	\
INNER JOIN Artist on Song.ArtistID = Artist.ROWID	\
WHERE Artist.IsDel = 0 AND Artist.Name like \"%%1%\"	\
GROUP BY Song.ArtistID	\
ORDER BY Artist.Name	\
LIMIT 0, 100	\
"

#define SQL_SEARCH_TRACK_LIST	"	\
SELECT	\
	Song.ROWID as id,	\
	Song.Name as title,	\
	Song.nov_time as time,	\
	Artist.Name as Artist,	\
	Album.ROWID as AlbumID,	\
	Album.Name as Album,	\
	Genre.Name as Genre	\
FROM Song	\
INNER JOIN Album on Song.AlbumID = Album.ROWID	\
INNER JOIN Artist on Song.ArtistID = Artist.ROWID	\
INNER JOIN Genre on Song.GenreID = Genre.ROWID	\
WHERE Album.IsDel = 0 AND Song.Name like \"%%1%\"	\
GROUP BY Song.ROWID	\
ORDER BY Song.Name	\
LIMIT 0, 100	\
"

////////////////////////////////////////////
/// queue list info
////////////////////////////////////////////

#define SQL_QUEUE_CATEGORY_INFO	"	\
SELECT	\
	Album.Name as Album,	\
	Album.ROWID as AlbumID,	\
	Artist.Name as Artist,	\
	Artist.ROWID as ArtistID	\
FROM Song	\
INNER JOIN Album on Song.AlbumID = Album.ROWID	\
INNER JOIN Artist on Song.ArtistID = Artist.ROWID	\
WHERE Song.IsDel = 0 and Song.ROWID = %1	\
"



#endif // SQLCONSTATNS_H
