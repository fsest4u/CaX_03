#ifndef SQLCONSTATNS_H
#define SQLCONSTATNS_H

////////////////////////////////////////////
/// My Music
////////////////////////////////////////////

// 카테고리별 개수 가져오기 --------------------------------------------------------------------------

#define SQL_MUSIC_DB_OVERVIEW	"	\
select	\
	(select count(*) from (select * from Song where Song.IsDel = 0 group by song.AlbumID)) as Album	\
	, (select count(*) from (select * from Song where Song.IsDel = 0 group by song.AlbumArtistID)) as AlbumArtist	\
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

#define SQL_ALBUM_LIST_FROM_SONG	"	\
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
group by Album.rowid	\
order by Album.%6 %7	\
limit %8, %9"

#define SQL_ALBUM_ARTIST_LIST_FROM_SONG	"	\
select	\
	AlbumArtist.ROWID as id	\
	, AlbumArtist.Name as title	\
	, AlbumArtist.favorite as favorite	\
	, AlbumArtist.rating as rating	\
	, count(AlbumArtist.ROWID) as count	\
from Song	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
where AlbumArtist.IsDel = 0	%1 %2 %3 %4 %5 \
group by AlbumArtist.rowid	\
order by AlbumArtist.%6 %7	\
limit %8, %9"

#define SQL_ARTIST_LIST_FROM_SONG	"	\
select	\
	Artist.ROWID as id	\
	, Artist.Name as title	\
	, Artist.favorite as favorite	\
	, Artist.rating as rating	\
	, count(Artist.ROWID) as count	\
from Song	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Artist.IsDel = 0	%1 %2 %3 %4 %5 \
group by Artist.rowid	\
order by Artist.%6 %7	\
limit %8, %9"

#define SQL_COMPOSER_LIST_FROM_SONG	"	\
select	\
	Composer.ROWID as id	\
	, Composer.Name as title	\
	, Composer.favorite as favorite	\
	, Composer.rating as rating	\
	, count(Composer.ROWID) as count	\
from Song	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
where Composer.IsDel = 0	%1 %2 %3 %4 %5 \
group by Composer.rowid	\
order by Composer.%6 %7	\
limit %8, %9"

#define SQL_GENRE_LIST_FROM_SONG	"	\
select	\
	Genre.ROWID as id	\
	, Genre.Name as title	\
	, Genre.favorite as favorite	\
	, Genre.rating as rating	\
	, count(Genre.ROWID) as count	\
from Song	\
inner join Genre on Song.GenreID = Genre.ROWID	\
where Genre.IsDel = 0	%1 %2 %3 %4 %5 \
group by Genre.rowid	\
order by Genre.%6 %7	\
limit %8, %9"

#define SQL_MOOD_LIST_FROM_SONG	"	\
select	\
	Mood.ROWID as id	\
	, Mood.Name as title	\
	, Mood.favorite as favorite	\
	, Mood.rating as rating	\
	, count(Mood.ROWID) as count	\
from Song	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Mood.IsDel = 0	%1 %2 %3 %4 %5 \
group by Mood.rowid	\
order by Mood.%6 %7	\
limit %8, %9"

#define SQL_FOLDER_LIST_FROM_SONG	"	\
select	\
	Folder.ROWID as id	\
	, Folder.Name as title	\
	, Folder.favorite as favorite	\
	, Folder.rating as rating	\
	, count(Folder.ROWID) as count	\
from Song	\
inner join Folder on Song.FolderID = Folder.ROWID	\
where Folder.IsDel = 0	%1 %2 %3 %4 %5 \
group by Folder.rowid	\
order by Folder.%6 %7	\
limit %8, %9"

#define SQL_YEAR_LIST_FROM_SONG	"	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_ALBUM_ARTIST_OVERVIEW	"	\
select	\
	AlbumArtist.Name as title	\
	, Artist.Name as Artist	\
	, AlbumArtist.favorite as favorite \
	, AlbumArtist.rating as rating \
	, count(*) as count	\
	, sum(song.nov_time) as total	\
	, Song.FileName as extension	\
	, Song.nov_Bitrate as bps	\
	, Song.nov_Samplerate as samplerate	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
where Song.IsDel = 0 and Song.AlbumArtistID = %1	\
"

#define SQL_ALBUM_ARTIST_TRACK_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 and Song.AlbumArtistID = %1	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
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
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 \
order by Song.%1 %2	\
limit %3, %4"

// --------------------------------------------------------------------------
// 카테고리별 앨범 목록 가져오기 ------------------------------------------------
// --------------------------------------------------------------------------

#define SQL_ALBUM_OF_CAT_LIST_FROM_SONG	"	\
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
where Album.IsDel = 0	%1 %2 %3 %4 %5 %10	\
group by Album.rowid	\
order by Album.%6 %7	\
limit %8, %9"

#define SQL_ARTIST_OF_CAT_LIST_FROM_SONG	"	\
select	\
	Artist.ROWID as id	\
	, Artist.Name as title	\
	, Artist.favorite as favorite	\
	, Artist.rating as rating	\
	, count(Artist.ROWID) as count	\
from Song	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
where Artist.IsDel = 0	%1 %2 %3 %4 %5 %10	\
group by Artist.rowid	\
order by Artist.%6 %7	\
limit %8, %9"

#define SQL_ALBUM_OF_ARTIST_CAT_LIST_FROM_SONG	"	\
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
where Album.IsDel = 0	%1 %2 %3 %4 %5 %10 %11	\
group by Album.rowid	\
order by Album.%6 %7	\
limit %8, %9"

// --------------------------------------------------------------------------
// 카테고리별 앨범의 트랙 목록 가져오기 ------------------------------------------------
// --------------------------------------------------------------------------

#define SQL_TRACK_LIST_OF_ALBUM_OF_CAT_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1 %6	\
order by Song.%2 %3	\
limit %4, %5"

#define SQL_TRACK_LIST_OF_ALBUM_OF_ARTIST_OF_CAT_LIST	"	\
select	\
	Song.ROWID as id	\
	, Song.Name as title	\
	, Song.Favorite as Favorite	\
	, Song.nov_time as time	\
	, Artist.Name as Artist	\
	, Album.Name as Album	\
	, Genre.Name as Genre	\
	, AlbumArtist.Name as AlbumArtist	\
	, Composer.Name as Composer	\
	, Song.Year as Year	\
	, Mood.Name as Mood	\
	, Song.Tempo as Tempo	\
	, Song.FileName as Format	\
	, Song.nov_Samplerate as SampleRate	\
	, Song.nov_Bitrate as BitRate	\
	, Song.Rating as Rating	\
from Song	\
inner join Album on Song.AlbumID = Album.ROWID	\
inner join Artist on Song.ArtistID = Artist.ROWID	\
inner join Genre on Song.GenreID = Genre.ROWID	\
inner join AlbumArtist on Song.AlbumArtistID = AlbumArtist.ROWID	\
inner join Composer on Song.ComposerID = Composer.ROWID	\
inner join Mood on Song.MoodID = Mood.ROWID	\
where Song.IsDel = 0 and Song.AlbumID = %1 %6 %7	\
order by Song.%2 %3	\
limit %4, %5"

// --------------------------------------------------------------------------
// Favorite & Rating ------------------------------------------------
// --------------------------------------------------------------------------

#define SQL_UPDATE_FAVORITE_OF_ALBUM		"update Album set Favorite=%1 where Album.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ALBUM_ARTIST	"update AlbumArtist set Favorite=%1 where AlbumArtist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_ARTIST		"update Artist set Favorite=%1 where Artist.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_GENRE		"update Genre set Favorite=%1 where Genre.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_COMPOSER		"update Composer set Favorite=%1 where Composer.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_MOOD			"update Mood set Favorite=%1 where Mood.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_FOLDER		"update Folder set Favorite=%1 where Folder.ROWID=%2"
#define SQL_UPDATE_FAVORITE_OF_SONG			"update Song set Favorite=%1 where Song.ROWID=%2"

#define SQL_UPDATE_RATING_OF_ALBUM			"update Album set Rating=%1 where Album.ROWID=%2"
#define SQL_UPDATE_RATING_OF_ALBUM_ARTIST	"update AlbumArtist set Rating=%1 where AlbumArtist.ROWID=%2"
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


////////////////////////////////////////////
/// Check category
////////////////////////////////////////////

#define SQL_CHECK_ALBUM	"	\
SELECT	\
	ROWID AS id	\
FROM Album	\
WHERE Album.IsDel = 0 AND Album.Name = \"%1\"	\
"

#define SQL_CHECK_ALBUM_ARTIST	"	\
SELECT	\
	ROWID AS id	\
FROM AlbumArtist	\
WHERE AlbumArtist.IsDel = 0 AND AlbumArtist.Name = \"%1\"	\
"

#define SQL_CHECK_ARTIST	"	\
SELECT	\
	ROWID AS id	\
FROM Artist	\
WHERE Artist.IsDel = 0 AND Artist.Name = \"%1\"	\
"

#define SQL_CHECK_GENRE	"	\
SELECT	\
	ROWID AS id	\
FROM Genre	\
WHERE Genre.IsDel = 0 AND Genre.Name = \"%1\"	\
"

#define SQL_CHECK_COMPOSER	"	\
SELECT	\
	ROWID AS id	\
FROM Composer	\
WHERE Composer.IsDel = 0 AND Composer.Name = \"%1\"	\
"

#define SQL_CHECK_MOOD	"	\
SELECT	\
	ROWID AS id	\
FROM Mood	\
WHERE Mood.IsDel = 0 AND Mood.Name = \"%1\"	\
"

////////////////////////////////////////////
/// Insert category
////////////////////////////////////////////

#define SQL_INSERT_ALBUM		"	\
INSERT INTO	\
	Album (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM Album	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"

#define SQL_INSERT_ALBUM_ARTIST		"	\
INSERT INTO	\
	AlbumArtist (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM AlbumArtist	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"

#define SQL_INSERT_ARTIST		"	\
INSERT INTO	\
	Artist (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM Artist	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"

#define SQL_INSERT_GENRE		"	\
INSERT INTO	\
	Genre (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM Genre	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"


#define SQL_INSERT_COMPOSER		"	\
INSERT INTO	\
	Composer (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM Composer	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"


#define SQL_INSERT_MOOD		"	\
INSERT INTO	\
	Mood (Name)	\
VALUES (\"%1\");		\
SELECT	\
	ROWID AS id	\
FROM Mood	\
ORDER BY ROWID DESC	\
LIMIT 1;	\
"


////////////////////////////////////////////
/// Update category
////////////////////////////////////////////

#define SQL_UPDATE_ALBUM		"UPDATE Song SET AlbumID = %1 WHERE Song.%2ID = %3"
#define SQL_UPDATE_ALBUM_ARTIST	"UPDATE Song SET AlbumArtistID =%1 WHERE Song.%2ID = %3"
#define SQL_UPDATE_ARTIST		"UPDATE Song SET ArtistID = %1 WHERE Song.%2ID = %3"
#define SQL_UPDATE_GENRE		"UPDATE Song SET GenreID = %1 WHERE Song.%2ID = %3"
#define SQL_UPDATE_COMPOSER		"UPDATE Song SET ComposerID = %1 WHERE Song.%2ID = %3"
#define SQL_UPDATE_MOOD			"UPDATE Song SET MoodID = %1 WHERE Song.%2ID = %3"


////////////////////////////////////////////
/// Insert or Ignore category all
////////////////////////////////////////////

#define SQL_INSERT_OR_IGNORE_ALBUM			"INSERT or IGNORE INTO Album (Name) VALUES (\"%1\");"
#define SQL_INSERT_OR_IGNORE_ALBUM_ARTIST	"INSERT or IGNORE INTO AlbumArtist (Name) VALUES (\"%1\");"
#define SQL_INSERT_OR_IGNORE_ARTIST			"INSERT or IGNORE INTO Artist (Name) VALUES (\"%1\");"
#define SQL_INSERT_OR_IGNORE_GENRE			"INSERT or IGNORE INTO Genre (Name) VALUES (\"%1\");"
#define SQL_INSERT_OR_IGNORE_COMPOSER		"INSERT or IGNORE INTO Composer (Name) VALUES (\"%1\");"
#define SQL_INSERT_OR_IGNORE_MOOD			"INSERT or IGNORE INTO Mood (Name) VALUES (\"%1\");"

////////////////////////////////////////////
/// Update category all
////////////////////////////////////////////

#define SQL_UPDATE_ALL		"	\
UPDATE	\
	Song	\
SET	\
	Name = \"%2\",	\
	Favorite = \"%3\",	\
	ArtistID = (SELECT Artist.ROWID FROM Artist WHERE Artist.Name = \"%4\"),	\
	AlbumID = (SELECT Album.ROWID FROM Album WHERE Album.Name = \"%5\"),	\
	GenreID = (SELECT Genre.ROWID FROM Genre WHERE Genre.Name = \"%6\"),	\
	AlbumArtistID = (SELECT AlbumArtist.ROWID FROM AlbumArtist WHERE AlbumArtist.Name = \"%7\"),	\
	ComposerID = (SELECT Composer.ROWID FROM Composer WHERE Composer.Name = \"%8\"),	\
	Year = \"%9\",	\
	MoodID = (SELECT Mood.ROWID FROM Mood WHERE Mood.Name = \"%10\")	\
WHERE	\
	Song.ROWID = \"%1\";	\
"

#endif // SQLCONSTATNS_H

