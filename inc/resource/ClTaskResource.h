
///////////////////////////////////////////////////////////////////////////////
// ID range is (23000~23500)
///////////////////////////////////////////////////////////////////////////////

#define ClTask_Resource_StartID							23000


/************************************************************************/
// 字符串

#define IDS_TASK										ClTask_Resource_StartID+1
#define IDS_NEW_TASK									ClTask_Resource_StartID+2
#define IDS_TaskManagement								ClTask_Resource_StartID+3
#define IDS_TaskPartManagement							ClTask_Resource_StartID+4
#define IDS_TaskSheetManagement							ClTask_Resource_StartID+5

// 按钮提示
#define IDS_AddTaskPartFromDataCenter                   ClTask_Resource_StartID+10
#define IDS_AddTaskPartFromDxfDwg                       ClTask_Resource_StartID+11
#define IDS_EditPartInfo								ClTask_Resource_StartID+12
#define IDS_DeletePartFromTask							ClTask_Resource_StartID+13
//

// 提示消息
#define IDS_MSG_PartExistInTask                         ClTask_Resource_StartID+50
#define IDS_MSG_NoPartIsSelected                        ClTask_Resource_StartID+51
#define IDS_MSG_NoPartIsSelectedToDelete                ClTask_Resource_StartID+52
#define IDS_MSG_MakeSureToDeletePart                    ClTask_Resource_StartID+53
#define IDS_MSG_NoPartToShow                            ClTask_Resource_StartID+54
#define IDS_MSG_CannotDeleteAllTaskSheet                ClTask_Resource_StartID+55
#define IDS_MSG_MakeSureToDeleteTaskSheet               ClTask_Resource_StartID+56
#define IDS_MSG_NoSheetIsSelectedToDelete               ClTask_Resource_StartID+57
#define IDS_MSG_MakeSureToDeletePart_2                  ClTask_Resource_StartID+58
#define IDS_MSG_MaterialStockWarning                    ClTask_Resource_StartID+59
#define IDS_MSG_PleaseSelectUnfinishedParts             ClTask_Resource_StartID+60
#define IDS_MSG_AllPartsfinished                        ClTask_Resource_StartID+61
#define IDS_MSG_NoPartIsSelectedToEdit                  ClTask_Resource_StartID+62
//

// names for commands
#define IDS_COMMAND_AutoNesting			                ClTask_Resource_StartID+100
//

// 一些名词
#define IDS_PartPlanningCount							ClTask_Resource_StartID+200
#define IDS_PartPlacedCount							    ClTask_Resource_StartID+201
#define IDS_RequiredMaterialQuantity				    ClTask_Resource_StartID+202
#define IDS_RemainingQuantity                           ClTask_Resource_StartID+203
#define IDS_PartNestingPriority                         ClTask_Resource_StartID+204
#define IDS_PartNestingRotateAngle                      ClTask_Resource_StartID+205
//
/************************************************************************/


/************************************************************************/
// 命令ID

#define IDC_AddTaskPartFromDataCenter                   ClTask_Resource_StartID+300
#define IDC_AddTaskPartFromDxfDwg                       ClTask_Resource_StartID+301
/************************************************************************/


/************************************************************************/
// 控件ID

#define IDC_TaskPartManagementBar                       ClTask_Resource_StartID+350
#define IDC_TaskSheetManagementBar                      ClTask_Resource_StartID+351
/************************************************************************/


/************************************************************************/
// 自定义消息

// 发这个消息要求任务视图打开任务中的最后一张板材。
#define WM_openLastSheetInTask	                        ClTask_Resource_StartID+400

// 发这个消息要求“任务视图”激活指定的“板材子视图”，消息参数会携带板材的ID。
#define WM_activeSheetSubView	                        ClTask_Resource_StartID+401

// 发这个消息要求“任务视图”删除指定的“板材子视图”，消息参数会携带板材子视图的索引。
#define WM_deleteSheetSubView	                        ClTask_Resource_StartID+402

// 当在任务零件对话框中选择或取消选择零件时，发这个消息给爷爷对话框。
#define WM_selectOrUnSelectPartInTaskPartDlg            ClTask_Resource_StartID+403

// 当在板材对话框中选择或取消选择板材时，发这个消息给爷爷对话框。
#define WM_selectOrUnSelectSheetInSheetDlg              ClTask_Resource_StartID+404

// 发这个消息给当前的任务视图，让它在当前的板材子视图上摆放零件，消息参数会携带零件的ID。
#define WM_placePartOnCurrentSheetSubView               ClTask_Resource_StartID+405

// 发送这个消息给任务视图，让他让板材管理器面板重新显示所有板材。
#define WM_reDisplayTaskSheets                          ClTask_Resource_StartID+407

// 发送这个消息给任务视图，让他更新修改轮廓刀具的面板。
#define WM_UpdateLoopToolPanel							ClTask_Resource_StartID+408
/************************************************************************/