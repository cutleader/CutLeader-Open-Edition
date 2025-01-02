#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓刀具的尺寸。
class ClCutFeature_Export ToolSizeModifyCommand : public CommandBase
{
public:
	ToolSizeModifyCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, double dNewToolSize);
	~ToolSizeModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;

	// 老的尺寸。
	std::vector<double> m_oldToolSizeValues;

	// 新的尺寸
	double m_dNewToolSize;
};

END_CUTLEADER_NAMESPACE()
