#include "StdAfx.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

SequenceData::SequenceData(RefPtDataPtr pRefPtData, CutSequenceListPtr pCutSequences)
{
	m_pRefPtData = pRefPtData;
	m_pCutSequences = pCutSequences;
}

SequenceData::~SequenceData(void)
{
}

END_CUTLEADER_NAMESPACE()
