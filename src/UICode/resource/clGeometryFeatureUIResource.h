
///////////////////////////////////////////////////////////////////////////////
// ID range is (27000~27100) 
///////////////////////////////////////////////////////////////////////////////

#define ClGeometryFeatureUI_Resource_StartID							27000

/************************************************************************/
/*                the string ID                                         */

#define IDS_LoopCount									ClGeometryFeatureUI_Resource_StartID+4
#define IDS_GeometrySize								ClGeometryFeatureUI_Resource_StartID+5
#define IDS_GeometryLength								ClGeometryFeatureUI_Resource_StartID+6
#define IDS_GeometryCount								ClGeometryFeatureUI_Resource_StartID+7
#define IDS_TextCount									ClGeometryFeatureUI_Resource_StartID+8
#define IDS_DrawText									ClGeometryFeatureUI_Resource_StartID+9
#define IDS_EditText									ClGeometryFeatureUI_Resource_StartID+10
#define IDS_Text										ClGeometryFeatureUI_Resource_StartID+11
#define IDS_SetLoopSize									ClGeometryFeatureUI_Resource_StartID+12

// 提示消息
#define IDS_MSG_CannotCreateText						ClGeometryFeatureUI_Resource_StartID+21
#define IDS_MSG_SelectTextLeftBottomPt					ClGeometryFeatureUI_Resource_StartID+22
/************************************************************************/


/************************************************************************/
// 创建的控件。

// used for "draw" panel.
#define IDC_GEOM_DRAW_TEXT								ClGeometryFeatureUI_Resource_StartID+50

// 右键菜单
#define IDC_SelectPattern_PopupMenu						ClGeometryFeatureUI_Resource_StartID+55
#define IDC_DeletePattern_PopupMenu						ClGeometryFeatureUI_Resource_StartID+56
#define IDC_CopyPattern_PopupMenu						ClGeometryFeatureUI_Resource_StartID+57
#define IDC_MovePattern_PopupMenu						ClGeometryFeatureUI_Resource_StartID+58
#define IDC_RotatePattern_PopupMenu						ClGeometryFeatureUI_Resource_StartID+59

// 几何特征属性面板上面的控件。
#define IDC_BTN_SetRectLinesSize						ClGeometryFeatureUI_Resource_StartID+65
/************************************************************************/


/************************************************************************/
// 自定义消息

// 发送这个消息给零件视图，要求进入选择几何特征动作。
#define WM_EnterSelectGeometryAction	    			ClGeometryFeatureUI_Resource_StartID+91 // 这个消息不会预先选择对象。
#define WM_EnterSelectGeometryAction_AndPreSelect	    ClGeometryFeatureUI_Resource_StartID+92 // 参数中携带要预先选中的几何轮廓ID集合的指针，消息接收者负责释放。
/************************************************************************/
