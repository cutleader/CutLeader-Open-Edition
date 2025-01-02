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
    // 板材中是否出现了新的零件，要是有的话，加入“任务零件”中。
    static bool CheckWhetherNewPartsInSheets(TaskItemPtr pTaskItem);

    // 克隆一份任务
    static TaskItemPtr CloneTaskItem(const TaskItem* pOldTaskItem); // 零件和板材都要克隆
    static TaskItemPtr CloneTaskItem_notClonePart(const TaskItem* pOldTaskItem);


    /************************************************************************/
    // 关于保存和加载任务对象。

    // 保存新任务对象到数据库。
	// 注:
	//  1) 这个函数会修改主数据库文件、新建任务数据库文件、新建板材数据库文件。
    //  2) 材料库存需要在调用这个函数前进行检查，一旦调用这个函数且库存不够，这个函数会增加库存。 
	static DataItemPtr SaveNewTaskItem(TaskItemPtr pTaskItem, FolderNodePtr pParentFolder);

    // 保存已有的任务对象到数据库。
    // 注:
    //  1) 材料库存需要在调用这个函数前进行检查，一旦调用这个函数且库存不够，这个函数会增加库存。
    static void SaveExistTaskItem(TaskItemPtr pTaskItem);

    // 删除任务对象，这个函数不删除主数据库中的产品项信息，其他关于任务的信息都会删除。
    static void DeleteTaskItemFromDB_notCareDataCenterItemInMainDB(const TaskItem* pTaskItem, bool bTransaction);

    // 从数据库加载任务对象。调用前需要确保这个任务已经存在于数据库中。
    static TaskItemPtr LoadTaskItemFromDb(LONGLONG iTaskItemID);
    static TaskItemPtr LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(LONGLONG iTaskItemID); // 为了数据中心的任务页面而加载的任务对象，就加载了页面上的那些内容。

    // 更新表格"clTask_usedPart"。
    static void SaveUsedPartInfo(DBConnectPtr pDBConnect, LONGLONG iTaskID, IntegerListPtr pPartIDList, BOOL bTransaction = TRUE);
    static void DeleteUsedPartInfo(DBConnectPtr pDBConnect, LONGLONG iTaskID, BOOL bTransaction = TRUE);

    // 得到引用了这个零件的所有任务信息。
    // 注:
    //   1) data in a DataProperty: 
    //      1) task id
    //      2) task name
    //      3) create date
    //      4) modify date
    //      5) task comment
    static BOOL GetTaskInfoList(DBConnectPtr pDBConnect, std::vector<DataProperty>& taskInfoList, LONGLONG iPartID);

    // 找出没有足够库存的材料，<材料信息，期望的张数>。
    // 注：
    //  1) pTaskItem是希望保存的任务。
    static std::vector<std::pair<DataProperty, int>> GetMaterialData_noEnoughStock(const TaskItem* pTaskItem);

	// 根据任务ID从数据库查询底图中使用的那个零件ID。
	static LONGLONG QueryPartUsedByCanvas(LONGLONG iTaskID);
    /************************************************************************/


    // 从“排版作业”生成报表。
    static ReportDataPtr GenerateTaskRpt(TaskItemPtr pTaskItem, const ReportTpl* pReportTpl);

    // 任务中是不是有板材丢失了材料或加工参数。
    static bool WhetherSomeSheetLostMaterial(LONGLONG iTaskItemID);
    static bool WhetherSomeSheetLostCamParam(LONGLONG iTaskItemID);

    // 任务中是否有未排完的零件
    static bool WhetherHaveUnfinishedParts(const TaskItem* pTaskItem);

private:
    // 任务数据库中是否有这个板材。
    static bool IsSheetInTaskDbFile(DBConnectPtr pTaskDbConnect, LONGLONG iSheetID);

    // 更新任务中零件的排版数量。
    static void UpdateNestedCount_ofTaskParts(TaskItemPtr pTaskItem);
};

END_CUTLEADER_NAMESPACE()
