#pragma once

#include "clPartLayoutCommon.h"
#include "PatternPosition.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �����������е��Ƶ����ϡ�
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

	// �����ݡ�
	std::vector<ILoopStartCutPtPtr> m_newSECutPts;
	std::vector<ILeadPtr> m_newLeadIns;
	std::vector<ILeadPtr> m_newLeadOuts;

	// �����ݡ�
	std::vector<ILoopStartCutPtPtr> m_oldSECutPts; // �ϵ����е���Ϣ��
	std::vector<ILeadPtr> m_oldLeadIns; // �ϵ���������Ϣ��
	std::vector<ILeadPtr> m_oldLeadOuts; // �ϵ���������Ϣ��
};

END_CUTLEADER_NAMESPACE()
