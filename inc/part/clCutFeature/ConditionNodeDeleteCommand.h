#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNode)

BEGIN_CUTLEADER_NAMESPACE()

// 从“轮廓刀具”上删除切割条件的命令。
class ClCutFeature_Export ConditionNodeDeleteCommand : public CommandBase
{
public:
	ConditionNodeDeleteCommand(const EditorData& editorData, LoopToolDataListPtr pLoopToolList, LONGLONG iDelCondTypeID);
	~ConditionNodeDeleteCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopToolDataListPtr m_pLoopToolList;

	// 要删除的条件节点。
	LONGLONG m_iDelCondTypeID;

	// 被删除的条件节点。
	// 注：
	//   1) 有的为空。
	std::vector<ConditionNodePtr> m_delConNodeList;
};

END_CUTLEADER_NAMESPACE()
