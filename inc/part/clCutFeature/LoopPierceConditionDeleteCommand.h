#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStaEndCutList)
DECLARE_CUTLEADER_CLASS(CondNode)

BEGIN_CUTLEADER_NAMESPACE()

// 从“回路切入点或切出点”上删除切割条件的命令。
class DelLoopPierceCond : public CommandBase
{
public:
	DelLoopPierceCond(EditorData editorData, LoopStaEndCutListPtr pLoopStaEndCutList, BOOL bModLeadIn, LONGLONG iDelCondTypeID);
	~DelLoopPierceCond(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopStaEndCutListPtr m_pLoopStaEndCutList;

	// 修改引入线的条件还是引出线的。
	BOOL m_bModLeadIn;

	// 要删除的条件节点。
	LONGLONG m_iDelCondTypeID;

	// 被删除的条件节点。
	// 注：
	//   1) 有的为空。
	std::vector<CondNodePtr> m_delConNodeList;
};

END_CUTLEADER_NAMESPACE()
