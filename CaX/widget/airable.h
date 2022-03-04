#ifndef ISERVICE_H
#define ISERVICE_H

// I-Service Type <<
#define iIServiceType_Qobuz           1
#define iIServiceType_Radios          2   // Airable Service      // 기본 i-Radio Service
#define iIServiceType_Podcasts        3                           // 기본 Podcast Service
#define iIServiceType_Tidal           4
#define iIServiceType_Napster         5
#define iIServiceType_Deezer          6
#define iIServiceType_HighResAudio    7
#define iIServiceType_AmazonMusic     8   // Airable Service
// I-Service Type >>

#define iAirableType_Mask_Dir         0x00000001      // Click 시, "Airable" -> "Url" request 를 통해, 하위 List View를 연다.
#define iAirableType_Mask_Sub         0x00000002      // 하위 항목으로 Head Icon 크기만큼 indent하여 List Item을 표시한다.

#define iAirableType_Mask_Track       0x00000004      // Play 가능한 Track
#define iAirableType_Mask_Program     0x00000008      // Play 가능한 Program, Program은 서비스가 무작이로 정해준 Stream Source를 차래로 Play한다.
													  // Playlist를 볼수 없으며, Seek, Next 등이 가능하며, Amazon Music의 경우 처음곡이 아니라면 Prev가 가능하다.
													  // 그래서, NowPlaying event에,  "Prev"= [true | false] tag 가 추가 되었다.
													  // Amazon Music의 경우 Rate기능 이 추가적으로 있는데,
													  // Positive, Neutral, Negative 의 설정이 가능하다.      -> NowPlaying event 참조 ( 10_PlayControl.cc )
#define iAirableType_Mask_Radio       0x00000010      // Play 가능한 i-Radio
#define iAirableType_Mask_Feed        0x00000020      // Play 가능한 Podcast

#define iAirableType_Mask_Play        0x00000040      // Play 가능
#define iAirableType_Mask_Art         0x00000080      // Cover View Top Menu 가능

#define iAirableType_Mask_Logout      0x00000100      // Logout Item
#define iAirableType_Mask_Record      0x00000200

#define iAirableType_Track_Feed       ( iAirableType_Mask_Track | iAirableType_Mask_Feed )
									  // Track과 Podcast는 일반적인 음원 List Play이다.       // 일반적 Play Menu Item 포함. ( Play New, Play Now, ... )
#define iAirableType_Playable         ( iAirableType_Mask_Track | iAirableType_Mask_Program | iAirableType_Mask_Radio | iAirableType_Mask_Feed )
									  // Track, Prgram, Radio, Podcast는 Playable함.
#define iAirableType_Play_Playable    ( iAirableType_Mask_Play | iAirableType_Mask_Track | iAirableType_Mask_Program | iAirableType_Mask_Radio | iAirableType_Mask_Feed )
									  // Playable과 관련된 모든 Mask





#endif // ISERVICE_H
