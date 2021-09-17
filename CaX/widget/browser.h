#ifndef BROWSER_H
#define BROWSER_H

#define PLAY_TOP        0x00000001      /* Top Menu에서 현재 folder의 모든 항목 Play 가능 */
#define PLAY_OPTION     0x00000002      /* Opntion Menu를 통해 선택 Play 가능 */
#define PLAY_CHECK      0x00000004      /* Check를 통해 Play 가능 */
#define PLAY_SELECT     0x00000008      /* Tap을 통해 Play 가능 */
#define CHEKC           0x00000010      /* Check 가능 */
#define FILE_MGR         0x00000020      /* Copy(Move,Delete), Rename 가능 */
#define READ_ONLY        0x00000040      /* Copy into, Move, Delete, Rename 불가능 */
#define SUB             0x00000080      /* Browser -> Folder Request 가능 */
#define DEV             0x00000100      /* Device or Device Root */

#define HDD             0x00000200      /* HDD Device */
#define USB             0x00000400      /* USB Device */
#define CD              0x00000800      /* CD  Device */
#define NET             0x00001000      /* NET Device */

#define ROOT            0x00002000      /* Root Dir */
#define DIR             0x00004000      /* Dir */
#define SONG            0x00008000      /* Song File 또는 pls,m3u,cue item */

#define PLS             0x00010000      /* pls,m3u,cue file 또는 item */
#define CUE             0x00020000      /* cue file 또는 하나의 음악파일을 2번 이상 참조하는 항목을 가진 cue file의 play item */
#define IRADIO          0x00040000      /* Pls item with internet streamming url */

#define ISOFILE         0x00080000      /* ISO File */
#define ISOROOT         0x00100000      /* ISO Root Path */
#define IMAGE           0x00200000      /* Image File */

#endif // BROWSER_H
