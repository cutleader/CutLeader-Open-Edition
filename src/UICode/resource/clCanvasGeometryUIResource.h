
///////////////////////////////////////////////////////////////////////////////
// ID range is (27300~27499) 
///////////////////////////////////////////////////////////////////////////////


#define ClCanvasGeometryUI_Resource_StartID							27300


/************************************************************************/
/*                the string ID                                         */

#define IDS_CanvasGeometry									ClCanvasGeometryUI_Resource_StartID+1
#define IDS_Canvas											ClCanvasGeometryUI_Resource_StartID+2
#define IDS_AddPatternFromDxf								ClCanvasGeometryUI_Resource_StartID+3
#define IDS_AddCanvasPattern2Task							ClCanvasGeometryUI_Resource_StartID+4
#define IDS_SelectPatternAdding2Task						ClCanvasGeometryUI_Resource_StartID+5

// 一些提示消息。
#define IDS_OUTPUT_SelectCanvasGeometry2Delete				ClCanvasGeometryUI_Resource_StartID+70
#define IDS_OUTPUT_SelectCanvasGeometry2Copy				ClCanvasGeometryUI_Resource_StartID+71
#define IDS_OUTPUT_SelectCanvasGeometry2Move				ClCanvasGeometryUI_Resource_StartID+72
#define IDS_OUTPUT_SelectCanvasGeometry2Rotate				ClCanvasGeometryUI_Resource_StartID+73
#define IDS_OUTPUT_SelectCanvasGeometry2Grid				ClCanvasGeometryUI_Resource_StartID+74
#define IDS_LoopDistanceDetect_ComCut						ClCanvasGeometryUI_Resource_StartID+75
#define IDS_PressKey_SetLoopDistance						ClCanvasGeometryUI_Resource_StartID+76
#define IDS_Find_StickPosition								ClCanvasGeometryUI_Resource_StartID+77
#define IDS_Find_AlignPosition								ClCanvasGeometryUI_Resource_StartID+78
#define IDS_Find_Stick_Align_Position						ClCanvasGeometryUI_Resource_StartID+79
#define IDS_OUTPUT_SelectCanvasGeometry2Edit				ClCanvasGeometryUI_Resource_StartID+80
#define IDS_OUTPUT_GridLoop									ClCanvasGeometryUI_Resource_StartID+81

// 一些命令名称。
#define IDS_COMMAND_AddLoop2Canvas							ClCanvasGeometryUI_Resource_StartID+90
#define IDS_COMMAND_CopyCanvasLoop							ClCanvasGeometryUI_Resource_StartID+91
#define IDS_COMMAND_MoveCanvasLoop							ClCanvasGeometryUI_Resource_StartID+92
#define IDS_COMMAND_RotateCanvasLoop						ClCanvasGeometryUI_Resource_StartID+93
#define IDS_COMMAND_GridCanvasLoop							ClCanvasGeometryUI_Resource_StartID+94
/************************************************************************/


/************************************************************************/
// 创建的控件。

#define IDC_CanvasPatternPropertyBar						ClCanvasGeometryUI_Resource_StartID+101

// 右键菜单
#define IDC_AddPatternFromDxf_PopupMenu						ClCanvasGeometryUI_Resource_StartID+105
#define IDC_AddCanvasPattern2Task_PopupMenu					ClCanvasGeometryUI_Resource_StartID+106
#define IDC_SelectCanvasPattern_PopupMenu					ClCanvasGeometryUI_Resource_StartID+107
#define IDC_DeleteCanvasPattern_PopupMenu					ClCanvasGeometryUI_Resource_StartID+108
#define IDC_CopyCanvasPattern_PopupMenu						ClCanvasGeometryUI_Resource_StartID+109
#define IDC_MoveCanvasPattern_PopupMenu						ClCanvasGeometryUI_Resource_StartID+110
#define IDC_RotateCanvasPattern_PopupMenu					ClCanvasGeometryUI_Resource_StartID+111
/************************************************************************/


/************************************************************************/
// 自定义消息

// 发送这个消息给底图视图，要求进入选择轮廓action，并不预选择对象。
#define WM_EnterCanvasLoopSelectAction	    				ClCanvasGeometryUI_Resource_StartID+150

// 发送这个消息给底图视图，要求进入选择几何特征(图元)action并预选择一个几何特征(图元)。
// 注：
// 1) 第一个参数是要选中几何特征的ID，消息接收者负责释放这个数据。
#define WM_EnterCanvasPatternSelectAction_PreSelectOnePattern	    ClCanvasGeometryUI_Resource_StartID+151

// 发送这个消息给底图视图，要求进入选择轮廓action并预选择轮廓和文字。
// 注：
// 1) 第一个参数是要选中轮廓的ID集合，第二个参数是这些轮廓中包含的文字，消息接收者负责释放这些数据。
#define WM_EnterCanvasLoopSelectAction_PreSelectLoopAndText	ClCanvasGeometryUI_Resource_StartID+152

// 发送这个消息给任务视图，把选中的几何特征定义为零件，存入数据库，并加到任务中。
// 第一个参数携带几何特征集合的指针, 第二个参数携带文字对象集合的指针，消息接收者要克隆一份。
#define WM_AddPattern2CurrentTask							ClCanvasGeometryUI_Resource_StartID+155
/************************************************************************/