#pragma once

#include <map>
#include "clCutFeatureCommon.h"
#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令负责修改轮廓实例的阴阳切方式。
// 注：
// 1) 传入的轮廓实例要的确可以被修改成emCutOffsetType方式，不能修改的事先要过滤掉。
//    如：emCutOffsetType是阳切，那么m_pLoopInstances中不要有阳切的轮廓。
class ClPartLayout_Export LoopInstCutSideChangeCommand : public CommandBase
{
	typedef std::pair<LONGLONG, LONGLONG> IdPair;

public:
	LoopInstCutSideChangeCommand(const EditorData& editorData, LoopInstanceListPtr pLoopInstances, PatternLoopCutOffsetType emCutOffsetType,
		LeadConfigItemPtr pLeadConfigItem);
	~LoopInstCutSideChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopInstanceListPtr m_pLoopInstances;
	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;

	// 记录之前的cut side和引线信息，用于undo。IdPair为<零件实例ID，几何轮廓ID>。
	std::map<IdPair, PatternLoopCutSideType> m_oldCutsideData;
	std::map<IdPair, ILeadPtr> m_oldLeadInData;
	std::map<IdPair, ILeadPtr> m_oldLeadOutData;
};

END_CUTLEADER_NAMESPACE()
