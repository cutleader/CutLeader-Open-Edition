#pragma once

#include "clPartLayoutCommon.h"
#include "PatternPosition.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令把起切点移到角上。
class ClPartLayout_Export StartCutPt2CornerCommand : public CommandBase
{
public:
	StartCutPt2CornerCommand(const EditorData& editorData, LoopStartCutDataListPtr pLoopStaEndCuts, ILoopStartCutPtPtr pLoopStartCutPt, ILeadPtr pLeadIn, ILeadPtr pLeadOut);
	virtual ~StartCutPt2CornerCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// the loop pierce which will be modified.
	LoopStartCutDataListPtr m_pLoopStaEndCuts;

	// 新数据。
	std::vector<ILoopStartCutPtPtr> m_newSECutPts;
	std::vector<ILeadPtr> m_newLeadIns;
	std::vector<ILeadPtr> m_newLeadOuts;

	// 老数据。
	std::vector<ILoopStartCutPtPtr> m_oldSECutPts; // 老的起切点信息。
	std::vector<ILeadPtr> m_oldLeadIns; // 老的引入线信息。
	std::vector<ILeadPtr> m_oldLeadOuts; // 老的引出线信息。
};

END_CUTLEADER_NAMESPACE()
