#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令负责修改几何轮廓的阴阳切方式。
// 注：
// 1) 传入的几何轮廓要的确可以被修改成emCutOffsetType方式，不能修改的事先要过滤掉。
//    如：emCutOffsetType是阳切，那么m_pLoopStartCutDataList中不要有阳切的轮廓。
class ClCutFeature_Export CutSideChangeCommand : public CommandBase
{
public:
    CutSideChangeCommand(const EditorData& editorData, LoopStartCutDataListPtr pLoopStartCutDataList, PatternLoopCutOffsetType emCutOffsetType,
		LeadConfigItemPtr pLeadConfigItem);
	~CutSideChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopStartCutDataListPtr m_pLoopStartCutDataList;
	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;

	// 记录之前的cut side和引线信息，用于undo
	std::map<LONGLONG, PatternLoopCutSideType> m_oldCutsideData;
	std::map<LONGLONG, ILeadPtr> m_oldLeadInData;
	std::map<LONGLONG, ILeadPtr> m_oldLeadOutData;
};

END_CUTLEADER_NAMESPACE()
