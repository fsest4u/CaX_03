#ifndef ISERVICE_H
#define ISERVICE_H

// I-Service Type <<
#define iIServiceType_Qobuz           1
#define iIServiceType_Radios          2   // Airable Service      // �⺻ i-Radio Service
#define iIServiceType_Podcasts        3                           // �⺻ Podcast Service
#define iIServiceType_Tidal           4
#define iIServiceType_Napster         5
#define iIServiceType_Deezer          6
#define iIServiceType_HighResAudio    7
#define iIServiceType_AmazonMusic     8   // Airable Service
// I-Service Type >>

#define iAirableType_Mask_Dir         0x00000001      // Click ��, "Airable" -> "Url" request �� ����, ���� List View�� ����.
#define iAirableType_Mask_Sub         0x00000002      // ���� �׸����� Head Icon ũ�⸸ŭ indent�Ͽ� List Item�� ǥ���Ѵ�.

#define iAirableType_Mask_Track       0x00000004      // Play ������ Track
#define iAirableType_Mask_Program     0x00000008      // Play ������ Program, Program�� ���񽺰� �����̷� ������ Stream Source�� ������ Play�Ѵ�.
													  // Playlist�� ���� ������, Seek, Next ���� �����ϸ�, Amazon Music�� ��� ó������ �ƴ϶�� Prev�� �����ϴ�.
													  // �׷���, NowPlaying event��,  "Prev"= [true | false] tag �� �߰� �Ǿ���.
													  // Amazon Music�� ��� Rate��� �� �߰������� �ִµ�,
													  // Positive, Neutral, Negative �� ������ �����ϴ�.      -> NowPlaying event ���� ( 10_PlayControl.cc )
#define iAirableType_Mask_Radio       0x00000010      // Play ������ i-Radio
#define iAirableType_Mask_Feed        0x00000020      // Play ������ Podcast

#define iAirableType_Mask_Play        0x00000040      // Play ����
#define iAirableType_Mask_Art         0x00000080      // Cover View Top Menu ����

#define iAirableType_Mask_Logout      0x00000100      // Logout Item
#define iAirableType_Mask_Record      0x00000200

#define iAirableType_Track_Feed       ( iAirableType_Mask_Track | iAirableType_Mask_Feed )
									  // Track�� Podcast�� �Ϲ����� ���� List Play�̴�.       // �Ϲ��� Play Menu Item ����. ( Play New, Play Now, ... )
#define iAirableType_Playable         ( iAirableType_Mask_Track | iAirableType_Mask_Program | iAirableType_Mask_Radio | iAirableType_Mask_Feed )
									  // Track, Prgram, Radio, Podcast�� Playable��.
#define iAirableType_Play_Playable    ( iAirableType_Mask_Play | iAirableType_Mask_Track | iAirableType_Mask_Program | iAirableType_Mask_Radio | iAirableType_Mask_Feed )
									  // Playable�� ���õ� ��� Mask





#endif // ISERVICE_H
