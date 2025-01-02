#pragma once

#include "ClTaskCommon.h"
#include "BusinessData.h"
#include "SubjectBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPart)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(Sheet)


BEGIN_CUTLEADER_NAMESPACE()


// 任务作为目标对象，提供这些给观察者的事件。
#define ID_EVENT_SheetChanged_byNesting   23542 // 任务中的板材由于自动排版变化了


// 任务对象
class ClTask_Export TaskItem : public BusinessData, public SubjectBase
{
public:
	TaskItem();
	virtual ~TaskItem(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_TaskItem; }

public: // get/set functions.
    TaskPartListPtr GetTaskPartList() const { return m_pTaskPartList; }
    SheetListPtr GetSheetList() const { return m_pSheetList; }

public:
    // 关于任务零件
    bool TaskPartExist(LONGLONG iPartID) const;
    TaskPartPtr GetTaskPartByPartID(LONGLONG iPartID) const;
    TaskPartPtr GetTaskPartByID(LONGLONG iTaskPartID) const;
    void DeletePartFromTask(LONGLONG iTaskPartID);
    int GetTaskPartIndex(LONGLONG iTaskPartID) const;

    // 关于任务中的板材
    void DeleteSheetFromTask(LONGLONG iSheetID);
    int GetSheetIndex(LONGLONG iSheetID) const;
    SheetPtr GetSheetByName(CString strName) const;

private:
    // 期望排（加工）的零件。
    TaskPartListPtr m_pTaskPartList;

    // 这个任务中的板材，它们不允许重名。
    SheetListPtr m_pSheetList;
};

END_CUTLEADER_NAMESPACE()
