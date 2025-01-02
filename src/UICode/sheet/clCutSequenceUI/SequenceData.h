#pragma once

#include "clCutSequenceUICommon.h"

DECLARE_CUTLEADER_CLASS(RefPtData)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SequenceData)

BEGIN_CUTLEADER_NAMESPACE()

// 板材上的工序数据。
class ClCutSequenceUI_Export SequenceData
{
public:
	SequenceData(RefPtDataPtr pRefPtData, CutSequenceListPtr pCutSequences);
	~SequenceData(void);

public:
	RefPtDataPtr GetRefPtData() const { return m_pRefPtData; }
	CutSequenceListPtr GetCutSequences() const { return m_pCutSequences; }

private:
	// 参考点数据。
	RefPtDataPtr m_pRefPtData;

	// 工序集合。
	CutSequenceListPtr m_pCutSequences;
};

END_CUTLEADER_NAMESPACE()
