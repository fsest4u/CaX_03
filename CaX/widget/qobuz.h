#ifndef QOBUZ_H
#define QOBUZ_H

#define iQobuzType_Mask_Artist                  0x00000001      // Qobuz Item Category mask <<
#define iQobuzType_Mask_Album                   0x00000002
#define iQobuzType_Mask_Track                   0x00000004
#define iQobuzType_Mask_Playlist                0x00000008      // Qobuz Item Category mask >>

#define iQobuzType_Mask_Search                  0x00000010      // �ֻ��� Menu Type <<
#define iQobuzType_Mask_Recommend               0x00000020      //   - Response Type��, Requst���� ���� Type mask�� ����� Response �Ѵ�.
#define iQobuzType_Mask_Favorite                0x00000040      //   - �� �� Top Menu�� ���� ���� �ߴ��� �˾ƾ� �Ѵ�.
#define iQobuzType_Mask_UserPlaylist            0x00000080      // �ֻ��� Menu Type >>

#define iQobuzType_Mask_Menu_Album              0x00000100      // ���� ����� � Type���� <<
#define iQobuzType_Mask_Menu_Playlist           0x00000200
#define iQobuzType_Mask_Menu_Genre              0x00000400      // Recommendations -> Album -> Genre ���� Menu Type���� �� �׻� �־�� �Ǵ� Flag
#define iQobuzType_Mask_Menu_Artist             0x00000800
#define iQobuzType_Mask_Menu_Track              0x00001000      // ���� ����� � Type���� >>

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

// iOS App ������ Top, Option Menu�� ó���ϱ� ���� ����ϴ� Flag <<
#define iQobuzMenu_AddFavorite      0x00000001      // Favorite�� �ƴ� ������ Artist, Album, Track Type�� "Add Favorite" ����
#define iQobuzMenu_AddPlaylist      0x00000002      // User Playlist�� �ƴ� ������ Track Type�� "Add Playlist" ����
#define iQobuzMenu_DelFavorite      0x00000004      // Favorite�� Menu Depth �� 2 �϶� "Delete Favorite" ����
#define iQobuzMenu_DelPlaylist      0x00000008      // User Playlist���� Playlist�� Delete ����, "Delete Playlist" �� �ѹ��� �ϳ��� ����, �� Option Menu�θ� ȣ��
#define iQobuzMenu_DelTrack         0x00000010      // User Playlist���� Track�� Delete ����
#define iQobuzMenu_Play             0x00000020      // Trach Type�� Play ����
#define iQobuzMenu_CoverView        0x00000040      // Artist, Album, Track, Playlist Type�� Cover View mode ���� ����
// iOS App ������ Top, Option Menu�� ó���ϱ� ���� ����ϴ� Flag ??

#define iQobuzMenu_AddFavorite_AddPlaylist_Play     ( iQobuzMenu_AddFavorite | iQobuzMenu_AddPlaylist | iQobuzMenu_Play )
#define iQobuzMenu_OptionMenu                       ( iQobuzMenu_AddFavorite_AddPlaylist_Play | iQobuzMenu_DelFavorite | iQobuzMenu_DelPlaylist | iQobuzMenu_DelTrack )


#endif // QOBUZ_H
