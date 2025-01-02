#pragma once

#include "clFrameCommon.h"

DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(SheetList)

BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export PartInplaceEditManager
{
public:
	// 在位编辑零件。
	static void OpenPartDocByInPlaceEditMode(CMultiDocTemplate* pPartDocTemplate);

	// 取消零件在位编辑模式。
	// 注：
	// 1) 简单的关掉零件，不保存任何修改。恢复其他文档的显示。
	static void CancelInPlaceEditMode();

	// 完成零件在位编辑模式。
	// 注：
	// 1) 如零件做了修改，则克隆一份，并替换掉指定任务中的零件。
	static void FinishInPlaceEditMode();

private:
	// 替换掉任务对象中对应的零件。
	// 注：
	// 1) 详细参考PartInplaceEditData的注释。
	// 2) 返回被更新了的板材对象。
	static SheetListPtr ReplacePartInTask(PartPtr pNewPart);

	// 根据是不是处于在位编辑模式来更新界面。
	static void UpdateGui();
};

END_CUTLEADER_NAMESPACE()
