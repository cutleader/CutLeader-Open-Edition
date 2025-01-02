#pragma once

#include "ClTaskCommon.h"
#include "CommandBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)


BEGIN_CUTLEADER_NAMESPACE()

// 继续排版任务中零件后得到的结果，送入这个command，用来执行修改。
// 注：
//  1）这个命令的undo/redo是由板材视图负责显示出来的。
//  2）这个命令的执行由任务视图负责执行。
//  3) 这个命令是在销毁任务视图时被任务视图销毁的。
class ClTask_Export NestTaskPartCommand : public CommandBase
{
public:
	NestTaskPartCommand(const EditorData& editorData, TaskItemPtr pTaskItem, TaskPartsNestResultPtr pTaskPartsNestResult);
	~NestTaskPartCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
    TaskItemPtr m_pTaskItem;

    // 排版结果
    TaskPartsNestResultPtr m_pTaskPartsNestResult;
};

END_CUTLEADER_NAMESPACE()
