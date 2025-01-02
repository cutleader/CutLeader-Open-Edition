#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// 给“轮廓刀具”上增加切割条件的命令。
class ClCutFeature_Export ConditionNodeAddCommand : public CommandBase
{
public:
	ConditionNodeAddCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, ConditionNodeListPtr pNewCondNodeList);
	~ConditionNodeAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的轮廓刀具特征。
	LoopToolDataListPtr m_pLoopToolList;

	// 要增加的条件节点。
	ConditionNodeListPtr m_pNewCondNodeList;
};

END_CUTLEADER_NAMESPACE()
