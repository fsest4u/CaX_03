#ifndef SETUP_H
#define SETUP_H

#define iAppSetupType_Mask_Enum			0x00000001
#define iAppSetupType_Mask_Select		0x00000002
#define iAppSetupType_Mask_List			0x00000004
#define iAppSetupType_Mask_Exec			0x00000008
#define iAppSetupType_Mask_FormSelect	0x00000010
#define iAppSetupType_Mask_App			0x00000020

#define iAppSetupType_Mask_Sub			0x00000040
#define iAppSetupType_Mask_PlayDisable	0x00000080
#define iAppSetupType_Mask_Disable		0x00000100
#define iAppSetupType_Mask_HideValue	0x00000200
#define iAppSetupType_Mask_Browser		0x00000400
#define iAppSetupType_Mask_Event		0x00000800

#define iAppSetupType_AllType				(	iAppSetupType_Mask_Enum | iAppSetupType_Mask_Select | iAppSetupType_Mask_Exec | iAppSetupType_Mask_List | \
												iAppSetupType_Mask_FormSelect | iAppSetupType_Mask_App | iAppSetupType_Mask_Event )

#define iAppSetupType_Enum_PlayDisable		( iAppSetupType_Mask_Enum | iAppSetupType_Mask_PlayDisable )
#define iAppSetupType_Switch_PlayDisable	( iAppSetupType_Mask_Switch | iAppSetupType_Mask_PlayDisable )
#define iAppSetupType_Select_PlayDisable	( iAppSetupType_Mask_Select | iAppSetupType_Mask_PlayDisable )
#define iAppSetupType_Exec_PlayDisable		( iAppSetupType_Mask_Exec | iAppSetupType_Mask_PlayDisable )

#define iAppSetupType_Enum_Sub				( iAppSetupType_Mask_Enum | iAppSetupType_Mask_Sub )
#define iAppSetupType_Exec_Sub				( iAppSetupType_Mask_Exec | iAppSetupType_Mask_Sub )
#define iAppSetupType_Exec_Sub_HideValue	( iAppSetupType_Mask_Exec | iAppSetupType_Mask_Sub | iAppSetupType_Mask_HideValue )

#define iAppSetupType_Enum_Select			( iAppSetupType_Mask_Enum | iAppSetupType_Mask_Select )

#define iSetupBtnAction_None		0
#define iSetupBtnAction_Request		1
#define iSetupBtnAction_Event		2
#define iSetupBtnAction_OpenWeb		3
#define iSetupBtnAction_Cancel		4
#define iSetupBtnAction_Max			iSetupBtnAction_Cancel

#define iSetupInput_Disp			1
#define iSetupInput_Label			2
#define iSetupInput_Hidden			3
#define iSetupInput_Slider			4
#define iSetupInput_List			5
#define iSetupInput_IP				6
#define iSetupInput_Password		7
#define iSetupInput_Text			8
#define iSetupInput_Time			9
#define iSetupInput_Switch			10
#define iSetupInput_CurrDate		11
#define iSetupInput_CurrTime		12

#endif // SETUP_H
