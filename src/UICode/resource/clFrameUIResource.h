
///////////////////////////////////////////////////////////////////////////////
// ID range is (26000~26099) 
///////////////////////////////////////////////////////////////////////////////

#define ClFrameUI_Resource_StartID							26000

/************************************************************************/
/*                the string ID                                         */

//左侧工具条
#define IDS_SetReferencePt						ClFrameUI_Resource_StartID+1
//

// ribbon工具条。
#define IDS_EDIT_INITIAL_TASK					ClFrameUI_Resource_StartID+10
#define IDS_EDIT_INITIAL_TASK_TIP				ClFrameUI_Resource_StartID+11
#define IDS_SetDefaultParam						ClFrameUI_Resource_StartID+12
#define IDS_SetDefaultParam_Tip					ClFrameUI_Resource_StartID+13
//

// 和前台cam相关。
#define IDS_Return2ForegroundCam_cancelEdit		ClFrameUI_Resource_StartID+15
#define IDS_Return2ForegroundCam_finishEdit		ClFrameUI_Resource_StartID+16
#define IDS_Return2ForegroundCam_NoSequence		ClFrameUI_Resource_StartID+17
#define IDS_Return2ForegroundCam_SomeLoopNoSequence		ClFrameUI_Resource_StartID+18
//

// 零件在位编辑相关。
#define IDS_LeavePartInplaceEdit_cancelEdit		ClFrameUI_Resource_StartID+20
#define IDS_LeavePartInplaceEdit_finishEdit		ClFrameUI_Resource_StartID+21
//

#define IDS_SaveTaskDoc							ClFrameUI_Resource_StartID+30
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

#define IDC_EDIT_INITIAL_TASK					ClFrameUI_Resource_StartID+50
#define IDC_SetDefaultParam						ClFrameUI_Resource_StartID+51
/************************************************************************/


/************************************************************************/
// custom messages.

// 发送这个消息给视图，视图会把光标当前位置显示在状态栏。
#define WM_displayCursorPositionOnStatusBar		ClFrameUI_Resource_StartID+90
// 发送这个消息给视图，视图会把当前工作模式显示在状态栏。
#define WM_displayCurrentWorkModeOnStatusBar	ClFrameUI_Resource_StartID+91
// 发送这个消息给视图，视图会把当前action的提示消息显示在状态栏。。
#define WM_displayPromptMessageOnStatusBar		ClFrameUI_Resource_StartID+92

// 给主线程发送这个消息，让它新建一个任务并加载dxf到底图，主线程负责释放消息数据。
#define WM_NewTaskAndAddDxf2Canvas				ClFrameUI_Resource_StartID+95
// 给主线程发送这个消息，让它打开指定任务中指定板材，主线程负责释放消息数据。
#define WM_OpenSpecifiedSheet					ClFrameUI_Resource_StartID+96
/************************************************************************/
