#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓刀具的颜色。
class ClCutFeature_Export ToolColorModifyCommand : public CommandBase
{
public:
	ToolColorModifyCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, COLORREF iNewToolColor);
	~ToolColorModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;

	// 老的颜色
	std::vector<COLORREF> m_oldToolColors;

	// 新的颜色
	COLORREF m_iNewToolColor;
};

END_CUTLEADER_NAMESPACE()
