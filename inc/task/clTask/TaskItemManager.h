#pragma once

#include "ClTaskCommon.h"
#include <vector>
#include "DataProperty.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(ReportTpl)


BEGIN_CUTLEADER_NAMESPACE()


class ClTask_Export TaskItemManager
{
public:
    // ������Ƿ�������µ������Ҫ���еĻ������롰����������С�
    static bool CheckWhetherNewPartsInSheets(TaskItemPtr pTaskItem);

    // ��¡һ������
    static TaskItemPtr CloneTaskItem(const TaskItem* pOldTaskItem); // ����Ͱ�Ķ�Ҫ��¡
    static TaskItemPtr CloneTaskItem_notClonePart(const TaskItem* pOldTaskItem);


    /************************************************************************/
    // ���ڱ���ͼ����������

    // ����������������ݿ⡣
	// ע:
	//  1) ����������޸������ݿ��ļ����½��������ݿ��ļ����½�������ݿ��ļ���
    //  2) ���Ͽ����Ҫ�ڵ����������ǰ���м�飬һ��������������ҿ�治����������������ӿ�档 
	static DataItemPtr SaveNewTaskItem(TaskItemPtr pTaskItem, FolderNodePtr pParentFolder);

    // �������е�����������ݿ⡣
    // ע:
    //  1) ���Ͽ����Ҫ�ڵ����������ǰ���м�飬һ��������������ҿ�治����������������ӿ�档
    static void SaveExistTaskItem(TaskItemPtr pTaskItem);

    // ɾ������������������ɾ�������ݿ��еĲ�Ʒ����Ϣ�����������������Ϣ����ɾ����
    static void DeleteTaskItemFromDB_notCareDataCenterItemInMainDB(const TaskItem* pTaskItem, bool bTransaction);

    // �����ݿ����������󡣵���ǰ��Ҫȷ����������Ѿ����������ݿ��С�
    static TaskItemPtr LoadTaskItemFromDb(LONGLONG iTaskItemID);
    static TaskItemPtr LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(LONGLONG iTaskItemID); // Ϊ���������ĵ�����ҳ������ص�������󣬾ͼ�����ҳ���ϵ���Щ���ݡ�

    // ���±��"clTask_usedPart"��
    static void SaveUsedPartInfo(DBConnectPtr pDBConnect, LONGLONG iTaskID, IntegerListPtr pPartIDList, BOOL bTransaction = TRUE);
    static void DeleteUsedPartInfo(DBConnectPtr pDBConnect, LONGLONG iTaskID, BOOL bTransaction = TRUE);

    // �õ�������������������������Ϣ��
    // ע:
    //   1) data in a DataProperty: 
    //      1) task id
    //      2) task name
    //      3) create date
    //      4) modify date
    //      5) task comment
    static BOOL GetTaskInfoList(DBConnectPtr pDBConnect, std::vector<DataProperty>& taskInfoList, LONGLONG iPartID);

    // �ҳ�û���㹻���Ĳ��ϣ�<������Ϣ������������>��
    // ע��
    //  1) pTaskItem��ϣ�����������
    static std::vector<std::pair<DataProperty, int>> GetMaterialData_noEnoughStock(const TaskItem* pTaskItem);

	// ��������ID�����ݿ��ѯ��ͼ��ʹ�õ��Ǹ����ID��
	static LONGLONG QueryPartUsedByCanvas(LONGLONG iTaskID);
    /************************************************************************/


    // �ӡ��Ű���ҵ�����ɱ���
    static ReportDataPtr GenerateTaskRpt(TaskItemPtr pTaskItem, const ReportTpl* pReportTpl);

    // �������ǲ����а�Ķ�ʧ�˲��ϻ�ӹ�������
    static bool WhetherSomeSheetLostMaterial(LONGLONG iTaskItemID);
    static bool WhetherSomeSheetLostCamParam(LONGLONG iTaskItemID);

    // �������Ƿ���δ��������
    static bool WhetherHaveUnfinishedParts(const TaskItem* pTaskItem);

private:
    // �������ݿ����Ƿ��������ġ�
    static bool IsSheetInTaskDbFile(DBConnectPtr pTaskDbConnect, LONGLONG iSheetID);

    // ����������������Ű�������
    static void UpdateNestedCount_ofTaskParts(TaskItemPtr pTaskItem);
};

END_CUTLEADER_NAMESPACE()
