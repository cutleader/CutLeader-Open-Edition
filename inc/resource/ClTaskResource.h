
///////////////////////////////////////////////////////////////////////////////
// ID range is (23000~23500)
///////////////////////////////////////////////////////////////////////////////

#define ClTask_Resource_StartID							23000


/************************************************************************/
// �ַ���

#define IDS_TASK										ClTask_Resource_StartID+1
#define IDS_NEW_TASK									ClTask_Resource_StartID+2
#define IDS_TaskManagement								ClTask_Resource_StartID+3
#define IDS_TaskPartManagement							ClTask_Resource_StartID+4
#define IDS_TaskSheetManagement							ClTask_Resource_StartID+5

// ��ť��ʾ
#define IDS_AddTaskPartFromDataCenter                   ClTask_Resource_StartID+10
#define IDS_AddTaskPartFromDxfDwg                       ClTask_Resource_StartID+11
#define IDS_EditPartInfo								ClTask_Resource_StartID+12
#define IDS_DeletePartFromTask							ClTask_Resource_StartID+13
//

// ��ʾ��Ϣ
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

// һЩ����
#define IDS_PartPlanningCount							ClTask_Resource_StartID+200
#define IDS_PartPlacedCount							    ClTask_Resource_StartID+201
#define IDS_RequiredMaterialQuantity				    ClTask_Resource_StartID+202
#define IDS_RemainingQuantity                           ClTask_Resource_StartID+203
#define IDS_PartNestingPriority                         ClTask_Resource_StartID+204
#define IDS_PartNestingRotateAngle                      ClTask_Resource_StartID+205
//
/************************************************************************/


/************************************************************************/
// ����ID

#define IDC_AddTaskPartFromDataCenter                   ClTask_Resource_StartID+300
#define IDC_AddTaskPartFromDxfDwg                       ClTask_Resource_StartID+301
/************************************************************************/


/************************************************************************/
// �ؼ�ID

#define IDC_TaskPartManagementBar                       ClTask_Resource_StartID+350
#define IDC_TaskSheetManagementBar                      ClTask_Resource_StartID+351
/************************************************************************/


/************************************************************************/
// �Զ�����Ϣ

// �������ϢҪ��������ͼ�������е����һ�Ű�ġ�
#define WM_openLastSheetInTask	                        ClTask_Resource_StartID+400

// �������ϢҪ��������ͼ������ָ���ġ��������ͼ������Ϣ������Я����ĵ�ID��
#define WM_activeSheetSubView	                        ClTask_Resource_StartID+401

// �������ϢҪ��������ͼ��ɾ��ָ���ġ��������ͼ������Ϣ������Я���������ͼ��������
#define WM_deleteSheetSubView	                        ClTask_Resource_StartID+402

// ������������Ի�����ѡ���ȡ��ѡ�����ʱ���������Ϣ��үү�Ի���
#define WM_selectOrUnSelectPartInTaskPartDlg            ClTask_Resource_StartID+403

// ���ڰ�ĶԻ�����ѡ���ȡ��ѡ����ʱ���������Ϣ��үү�Ի���
#define WM_selectOrUnSelectSheetInSheetDlg              ClTask_Resource_StartID+404

// �������Ϣ����ǰ��������ͼ�������ڵ�ǰ�İ������ͼ�ϰڷ��������Ϣ������Я�������ID��
#define WM_placePartOnCurrentSheetSubView               ClTask_Resource_StartID+405

// ���������Ϣ��������ͼ�������ð�Ĺ��������������ʾ���а�ġ�
#define WM_reDisplayTaskSheets                          ClTask_Resource_StartID+407

// ���������Ϣ��������ͼ�����������޸��������ߵ���塣
#define WM_UpdateLoopToolPanel							ClTask_Resource_StartID+408
/************************************************************************/