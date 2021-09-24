#ifndef BROWSER_H
#define BROWSER_H


#define iFolderType_Mask_Play_Top		0x00000001		/* Top Menu에서 현재 folder의 모든 항목 Play 가능 */
#define iFolderType_Mask_Play_Option	0x00000002		/* Opntion Menu를 통해 선택 Play 가능 */
#define iFolderType_Mask_Play_Check		0x00000004		/* Check를 통해 Play 가능 */
#define iFolderType_Mask_Play_Select	0x00000008		/* Tap을 통해 Play 가능 */
#define iFolderType_Mask_Check			0x00000010		/* Check 가능 */
#define iFolderType_Mask_FileMgr		0x00000020		/* Copy(Move,Delete), Rename 가능 */
#define iFolderType_Mask_ReadOnly		0x00000040		/* Copy into, Move, Delete, Rename 불가능 */
#define iFolderType_Mask_Sub			0x00000080		/* Browser -> Folder Request 가능 */
#define iFolderType_Mask_Dev			0x00000100		/* Device or Device Root */

#define iFolderType_Mask_Hdd			0x00000200		/* HDD Device */
#define iFolderType_Mask_Usb			0x00000400		/* USB Device */
#define iFolderType_Mask_Cd				0x00000800		/* CD  Device */
#define iFolderType_Mask_Net			0x00001000		/* NET Device */
#define iFolderType_Mask_Upnp			0x00002000		/* NET Device */

#define iFolderType_Mask_Root			0x00004000		/* Root Dir */
#define iFolderType_Mask_Dir			0x00008000		/* Dir */
#define iFolderType_Mask_Song			0x00010000		/* Song File 또는 pls,m3u,cue item */

#define iFolderType_Mask_Pls			0x00020000		/* pls,m3u,cue file 또는 item */
#define iFolderType_Mask_Cue			0x00040000		/* cue file 또는 하나의 음악파일을 2번 이상 참조하는 항목을 가진 cue file의 play item */
#define iFolderType_Mask_IRadio			0x00080000		/* Pls item with internet streamming url */
#define iFolderType_Mask_IsoFile		0x00100000		/* ISO File */
#define iFolderType_Mask_IsoRoot		0x00200000		/* ISO Root Path */
#define iFolderType_Mask_Image			0x00400000		/* Image File */
#define iFolderType_Mask_File			0x00800000		/* File : zip, pkg */
#define iFolderType_Mask_Scan			0x01000000		/* Scan Path */
#define iFolderType_Mask_Media			0x02000000		/* Storage Media Mnt Path */
#define iFolderType_Mask_FilePath		0x04000000		/* File Path */

#define iFolderType_Dev					( iFolderType_Mask_Dev | iFolderType_Mask_Sub )		// Browser Top

#define iFolderType_DevNet				( iFolderType_Dev | iFolderType_Mask_Net )			// Net Share list : Real path /mnt/NET
#define iFolderType_DevUpnp				( iFolderType_Dev | iFolderType_Mask_Upnp )			// UPnP list : UPnP -> g_Global.lpUpnpClientInfo

// << RoootDir
#define iFolderType_RootDir				( iFolderType_Dev | iFolderType_Mask_Root | iFolderType_Mask_Dir )
#define iFolderType_HddRootDir			( iFolderType_RootDir | iFolderType_Mask_Hdd )									// Real path is /mnt/HDD?
#define iFolderType_UsbRootDir			( iFolderType_RootDir | iFolderType_Mask_Usb )									// Real path is /mnt/USB?
#define iFolderType_CdRootDir			( iFolderType_RootDir | iFolderType_Mask_Cd | iFolderType_Mask_ReadOnly )		// Real path is /mnt/CD1
#define iFolderType_NetRootDir			( iFolderType_RootDir | iFolderType_Mask_Net )									// Real Path is /mnt/NET/???
#define iFolderType_UpnpRootDir			( iFolderType_RootDir | iFolderType_Mask_Upnp | iFolderType_Mask_ReadOnly )		// UPnP -> g_Global.lpUpnpClientInfo
// >> RootDir

#define iFolderType_Dir					( iFolderType_Mask_Dir | iFolderType_Mask_Play_Top | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Check | iFolderType_Mask_Sub \
										| iFolderType_Mask_Check | iFolderType_Mask_FileMgr)
#define iFolderType_Song				( iFolderType_Mask_Song | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Check | iFolderType_Mask_Play_Select | iFolderType_Mask_Check | iFolderType_Mask_FileMgr )
#define iFolderType_Pls					( iFolderType_Mask_Pls | iFolderType_Mask_Play_Top | iFolderType_Mask_Play_Option | iFolderType_Mask_Sub | iFolderType_Mask_Check | iFolderType_Mask_FileMgr )
#define iFolderType_PlsSong				( iFolderType_Mask_Pls | iFolderType_Mask_Song | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Check | iFolderType_Mask_Play_Select \
										| iFolderType_Mask_Check | iFolderType_Mask_ReadOnly )
#define iFolderType_Cue					( iFolderType_Pls | iFolderType_Mask_Cue )
#define iFolderType_CuePlsSong			( iFolderType_Mask_Pls | iFolderType_Mask_Cue | iFolderType_Mask_Song | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Select | iFolderType_Mask_ReadOnly )
#define iFolderType_IRadio				( iFolderType_Mask_Pls | iFolderType_Mask_IRadio | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Select | iFolderType_Mask_ReadOnly )
#define iFolderType_IsoFile				( iFolderType_Mask_IsoFile | iFolderType_Mask_Check | iFolderType_Mask_FileMgr )
#define iFolderType_IsoRoot				( iFolderType_Mask_IsoRoot | iFolderType_Dir | iFolderType_Mask_ReadOnly )
#define iFolderType_Image				( iFolderType_Mask_Image | iFolderType_Mask_Check | iFolderType_Mask_FileMgr )

#define iFolderType_UpnpDir				( iFolderType_Mask_Upnp | iFolderType_Mask_Dir | iFolderType_Mask_Sub | iFolderType_Mask_ReadOnly )
#define iFolderType_UpnpSong			( iFolderType_Mask_Upnp | iFolderType_Mask_Song | iFolderType_Mask_Play_Option | iFolderType_Mask_Play_Check | iFolderType_Mask_Play_Select \
																| iFolderType_Mask_Check | iFolderType_Mask_ReadOnly )

#define iFolderType_Mask_Play_Select_Option		( iFolderType_Mask_Play_Select | iFolderType_Mask_Play_Option )
#define iFolderType_Mask_PlsSong				( iFolderType_Mask_Pls | iFolderType_Mask_Song )					/* Rename시 확장자 제거하고 입력 받은후 Rename Request시 확장자 추가하여 Request함 */

#define iBrowserMode_Mask_Dir			0x00000001
#define iBrowserMode_Mask_Image			0x00000002
#define iBrowserMode_Mask_File			0x00000004

#define iBrowserMode_Mask_Copy			0x00000008
#define iBrowserMode_Mask_Move			0x00000010
#define iBrowserMode_Mask_Export		0x00000020
#define iBrowserMode_Mask_Option		0x00000040
#define iBrowserMode_Mask_Top			0x00000080
//#define iBrowserMode_Mask_Check		0x00000100

#define iBrowserMode_Mask_Copy_Move		( iBrowserMode_Mask_Copy | iBrowserMode_Mask_Move )

#define iBrowserMode_CopyOption			(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Copy | iBrowserMode_Mask_Option )
#define iBrowserMode_CopyTop			(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Copy | iBrowserMode_Mask_Top )

#define iBrowserMode_MoveOption			(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Move | iBrowserMode_Mask_Option )
#define iBrowserMode_MoveTop			(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Move | iBrowserMode_Mask_Top )

#define iBrowserMode_ExportOption		(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Export | iBrowserMode_Mask_Option )
#define iBrowserMode_ExportTop			(iBrowserMode_Mask_Dir | iBrowserMode_Mask_Export | iBrowserMode_Mask_Top )

#define Is_Song_File(x)					( ((x) & iFolderType_Mask_Song) && !((x) & iFolderType_Mask_Pls) )
#define Is_Song_Image_File(x)			( Is_Song_File(x) || ( (x) & iFolderType_Mask_Image ) )
#define Is_Song_Dir(x)					( Is_Song_File(x) || ( (x) & iFolderType_Mask_Dir ) )
#endif // BROWSER_H
