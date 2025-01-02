
///////////////////////////////////////////////////////////////////////////////
// ID range is (25100~25199)
///////////////////////////////////////////////////////////////////////////////

#define ClPartLayoutUI_Resource_StartID							25100

/************************************************************************/
/*                the string ID                                         */

// 消息提示
#define IDS_MSG_EDIT_LOOP_TOOL_2			                    ClPartLayoutUI_Resource_StartID+1
#define IDS_MSG_AssignedLoopTool			                    ClPartLayoutUI_Resource_StartID+2
#define IDS_MSG_AssignedMicroJoint			                    ClPartLayoutUI_Resource_StartID+3
#define IDS_MSG_AssignedCornerFeature			                ClPartLayoutUI_Resource_StartID+4
#define IDS_MSG_AssignedStartCutPtAndLeadData			        ClPartLayoutUI_Resource_StartID+5
#define IDS_MSG_ResetSequence_OK			                    ClPartLayoutUI_Resource_StartID+6

#define IDS_CannotPlacePartOnCanvas								ClPartLayoutUI_Resource_StartID+15
#define IDS_DonotCutLoopInstances_Tip							ClPartLayoutUI_Resource_StartID+16
#define IDS_CutLoopInstances_Tip								ClPartLayoutUI_Resource_StartID+17

#define IDS_PressKey_SetPartDistance							ClPartLayoutUI_Resource_StartID+20
#define IDS_MSG_SelectOnePartToEditPartData						ClPartLayoutUI_Resource_StartID+21
#define IDS_MSG_WhetherInplaceEditPart							ClPartLayoutUI_Resource_StartID+22
#define IDS_MSG_InplaceEditPart_EmptyPart						ClPartLayoutUI_Resource_StartID+23
//

#define IDS_InPlaceEditPart										ClPartLayoutUI_Resource_StartID+40
#define IDS_PartInstanceCount									ClPartLayoutUI_Resource_StartID+41
#define IDS_PartInstanceSize									ClPartLayoutUI_Resource_StartID+42
#define IDS_LoopInstanceProperty								ClPartLayoutUI_Resource_StartID+43
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

#define IDC_ChangeLoopTool_Bar									ClPartLayoutUI_Resource_StartID+60
#define IDC_InPlaceEditPart_PopupMenu							ClPartLayoutUI_Resource_StartID+61
#define	IDC_PROPBAR_LoopInstance								ClPartLayoutUI_Resource_StartID+62
/************************************************************************/


/************************************************************************/
// 自定义消息

// send the message to sheet view to assign the tool to the selected loop instances,
// the message will carry the tool index(-1 means no-cutting selected loops, -2 means cutting selected loops).
#define WM_TellSheetView_toChangeToolOfSelectedLoopInstances    ClPartLayoutUI_Resource_StartID+80

// 给主线程发送这个消息，让它打开指定零件进行在位编辑，在位编辑的含义请参考PartInplaceEditData中的注释，参数通过PartInplaceEditData来传递。
#define WM_InPlaceEditPart										ClPartLayoutUI_Resource_StartID+81
/************************************************************************/
