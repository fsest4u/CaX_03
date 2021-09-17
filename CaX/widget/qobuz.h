#ifndef QOBUZ_H
#define QOBUZ_H

#define iQobuzType_Mask_Artist                  0x00000001      // Qobuz Item Category mask <<
#define iQobuzType_Mask_Album                   0x00000002
#define iQobuzType_Mask_Track                   0x00000004
#define iQobuzType_Mask_Playlist                0x00000008      // Qobuz Item Category mask >>

#define iQobuzType_Mask_Search                  0x00000010      // 최상휘 Menu Type <<
#define iQobuzType_Mask_Recommend               0x00000020      //   - Response Type에, Requst에서 받은 Type mask를 살려서 Response 한다.
#define iQobuzType_Mask_Favorite                0x00000040      //   - 어 떤 Top Menu로 부터 시작 했는지 알아야 한다.
#define iQobuzType_Mask_UserPlaylist            0x00000080      // 최상휘 Menu Type >>

#define iQobuzType_Mask_Menu_Album              0x00000100      // 현재 목록이 어떤 Type인지 <<
#define iQobuzType_Mask_Menu_Playlist           0x00000200
#define iQobuzType_Mask_Menu_Genre              0x00000400      // Recommendations -> Album -> Genre 이하 Menu Type에서 는 항상 있어야 되는 Flag
#define iQobuzType_Mask_Menu_Artist             0x00000800
#define iQobuzType_Mask_Menu_Track              0x00001000      // 현재 목록이 어떤 Type인지 >>

#define iQobuzType_Artist_Album                 ( iQobuzType_Mask_Artist | iQobuzType_Mask_Album )                                  // Favorite Cat
#define iQobuzType_Artist_Album_Track           ( iQobuzType_Mask_Artist | iQobuzType_Mask_Album | iQobuzType_Mask_Track )          // Favorite Cat

#define iQobuzType_Artist_Playlist              ( iQobuzType_Mask_Artist | iQobuzType_Mask_Playlist )
#define iQobuzType_Artist_Album_Playlist        ( iQobuzType_Mask_Artist | iQobuzType_Mask_Album | iQobuzType_Mask_Playlist )
#define iQobuzType_Artist_Album_Track_Playlist  ( iQobuzType_Mask_Artist | iQobuzType_Mask_Album | iQobuzType_Mask_Track | iQobuzType_Mask_Playlist )

#define iQobuzType_Recommend_Album              ( iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album )
#define iQobuzType_Recommend_Playlist           ( iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Playlist )
#define iQobuzType_Recommend_Album_Genre        ( iQobuzType_Mask_Recommend | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Genre )

#define iQobuzType_Favorite_Artist              ( iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Artist )
#define iQobuzType_Favorite_Album               ( iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Album )
#define iQobuzType_Favorite_Track               ( iQobuzType_Mask_Favorite | iQobuzType_Mask_Menu_Track )
#define iQobuzType_Menu_Artist_Album_Track      ( iQobuzType_Mask_Menu_Artist | iQobuzType_Mask_Menu_Album | iQobuzType_Mask_Menu_Track )

// iOS App 에서만 Top, Option Menu를 처리하기 위해 사용하는 Flag <<
#define iQobuzMenu_AddFavorite      0x00000001      // Favorite이 아닌 곳에서 Artist, Album, Track Type는 "Add Favorite" 가능
#define iQobuzMenu_AddPlaylist      0x00000002      // User Playlist가 아닌 곳에서 Track Type은 "Add Playlist" 가능
#define iQobuzMenu_DelFavorite      0x00000004      // Favorite의 Menu Depth 가 2 일때 "Delete Favorite" 가능
#define iQobuzMenu_DelPlaylist      0x00000008      // User Playlist에서 Playlist는 Delete 가능, "Delete Playlist" 는 한번에 하나만 가능, 즉 Option Menu로만 호출
#define iQobuzMenu_DelTrack         0x00000010      // User Playlist에서 Track는 Delete 가능
#define iQobuzMenu_Play             0x00000020      // Trach Type만 Play 가능
#define iQobuzMenu_CoverView        0x00000040      // Artist, Album, Track, Playlist Type는 Cover View mode 변경 가능
// iOS App 에서만 Top, Option Menu를 처리하기 위해 사용하는 Flag ??

#define iQobuzMenu_AddFavorite_AddPlaylist_Play     ( iQobuzMenu_AddFavorite | iQobuzMenu_AddPlaylist | iQobuzMenu_Play )
#define iQobuzMenu_OptionMenu                       ( iQobuzMenu_AddFavorite_AddPlaylist_Play | iQobuzMenu_DelFavorite | iQobuzMenu_DelPlaylist | iQobuzMenu_DelTrack )


#endif // QOBUZ_H
