#ifndef BROWSER_H
#define BROWSER_H

#define PLAY_TOP        0x00000001      /* Top Menu���� ���� folder�� ��� �׸� Play ���� */
#define PLAY_OPTION     0x00000002      /* Opntion Menu�� ���� ���� Play ���� */
#define PLAY_CHECK      0x00000004      /* Check�� ���� Play ���� */
#define PLAY_SELECT     0x00000008      /* Tap�� ���� Play ���� */
#define CHEKC           0x00000010      /* Check ���� */
#define FILE_MGR         0x00000020      /* Copy(Move,Delete), Rename ���� */
#define READ_ONLY        0x00000040      /* Copy into, Move, Delete, Rename �Ұ��� */
#define SUB             0x00000080      /* Browser -> Folder Request ���� */
#define DEV             0x00000100      /* Device or Device Root */

#define HDD             0x00000200      /* HDD Device */
#define USB             0x00000400      /* USB Device */
#define CD              0x00000800      /* CD  Device */
#define NET             0x00001000      /* NET Device */

#define ROOT            0x00002000      /* Root Dir */
#define DIR             0x00004000      /* Dir */
#define SONG            0x00008000      /* Song File �Ǵ� pls,m3u,cue item */

#define PLS             0x00010000      /* pls,m3u,cue file �Ǵ� item */
#define CUE             0x00020000      /* cue file �Ǵ� �ϳ��� ���������� 2�� �̻� �����ϴ� �׸��� ���� cue file�� play item */
#define IRADIO          0x00040000      /* Pls item with internet streamming url */

#define ISOFILE         0x00080000      /* ISO File */
#define ISOROOT         0x00100000      /* ISO Root Path */
#define IMAGE           0x00200000      /* Image File */

#endif // BROWSER_H
