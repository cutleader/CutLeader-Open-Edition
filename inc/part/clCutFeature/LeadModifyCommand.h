#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LeadList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“引入或引出线”的命令。
// 注：
//   1) 修改的方式是替换掉老的“引入引出线”。
class ClCutFeature_Export LeadModifyCommand : public CommandBase
{
public:
	LeadModifyCommand(EditorData editorData, LeadInOutType iLeadType, LoopStartCutDataListPtr pLoopStaEndCutList, LeadListPtr pLeadList);
	~LeadModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 想修改引入线还是引出线。
	LeadInOutType m_iLeadType;

	// 要修改的“回路起切”特征。
	LoopStartCutDataListPtr m_pLoopStaEndCutList;

	// 新的引入或引出线集合，里面的每个元素都相同。
	LeadListPtr m_pLeadList;

	// 缓存老的引入或引出线集合。
	LeadListPtr m_pOldLeadList;
};

END_CUTLEADER_NAMESPACE()
