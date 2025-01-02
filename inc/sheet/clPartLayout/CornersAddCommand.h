#pragma once

#include "clCutFeatureCommon.h"
#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// 增加多个角特征的命令。
class ClPartLayout_Export CornersAddCommand : public CommandBase
{
public:
	CornersAddCommand(const EditorData& editorData, const std::vector<std::pair<LoopCornerDataPtr, ICornerPtr>>& cornerDataList);
	virtual ~CornersAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	std::vector<std::pair<LoopCornerDataPtr, ICornerPtr>> m_cornerDataList;
};

END_CUTLEADER_NAMESPACE()
