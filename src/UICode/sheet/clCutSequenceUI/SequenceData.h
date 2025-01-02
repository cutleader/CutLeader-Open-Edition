#pragma once

#include "clCutSequenceUICommon.h"

DECLARE_CUTLEADER_CLASS(RefPtData)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SequenceData)

BEGIN_CUTLEADER_NAMESPACE()

// ����ϵĹ������ݡ�
class ClCutSequenceUI_Export SequenceData
{
public:
	SequenceData(RefPtDataPtr pRefPtData, CutSequenceListPtr pCutSequences);
	~SequenceData(void);

public:
	RefPtDataPtr GetRefPtData() const { return m_pRefPtData; }
	CutSequenceListPtr GetCutSequences() const { return m_pCutSequences; }

private:
	// �ο������ݡ�
	RefPtDataPtr m_pRefPtData;

	// ���򼯺ϡ�
	CutSequenceListPtr m_pCutSequences;
};

END_CUTLEADER_NAMESPACE()
