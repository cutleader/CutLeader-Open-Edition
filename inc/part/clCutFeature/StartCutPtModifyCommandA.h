#pragma once

#include "clCutFeatureCommon.h"
#include "PatternPosition.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)

BEGIN_CUTLEADER_NAMESPACE()

// �����������е��Ƶ����ϻ��ߴӽ��ϵ�����ͼ���ϡ�
class ClCutFeature_Export StartCutPtModifyCommandA : public CommandBase
{
public:
	StartCutPtModifyCommandA(EditorData editorData, LoopStartCutDataPtr pLoopStartCutData, ILoopStartCutPtPtr pNewSECutPt, ILeadPtr pLeadIn, ILeadPtr pLeadOut);
	~StartCutPtModifyCommandA(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// the loop pierce which will be modified.
	LoopStartCutDataPtr m_pLoopStartCutData;

	// the start/end cut point info.
	ILoopStartCutPtPtr m_pNewSECutPt;

	// the lead info.
	ILeadPtr m_pLeadIn;
	ILeadPtr m_pLeadOut;
};

END_CUTLEADER_NAMESPACE()
