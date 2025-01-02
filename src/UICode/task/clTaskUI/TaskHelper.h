#pragma once

#include "ClTaskUICommon.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPart)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(FolderNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export TaskHelper
{
public:
	// 根据几何特征创建零件，并加入到任务。
	// 注：
	// 1) 如果pTextStructs中有数据，那么这些文字对象包含的几何特征也在pPatterns中。
	// 2) 文字对应的几何特征和几何轮廓可以通过文字参数重新生成。
	static void CreatePartAndAdd2Task(const PatternList* pPatterns, const TextStructList* pTextStructs, TaskItemPtr pTaskItem);

	// 这个函数检查任务结构是不是合法。
	// 注：
	// 1) 这个函数是为了以防万一，正常情况下任务的结构不会出问题。可以在打开任务后和保存任务前调用这个函数进行检查。
	static void CheckTaskItem(const TaskItem* pTaskItem);

	// 加载任务对象。
	// 注：
	// 1) 和TaskItemManager::LoadTaskItemFromDb的区别是，这个函数还检查了板材中的轮廓加工工艺。
	static TaskItemPtr LoadTaskItem(long long iTaskID);
};

END_CUTLEADER_NAMESPACE()
