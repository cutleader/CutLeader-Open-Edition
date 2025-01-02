#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(RefPtData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export RefPtData : public ClData
{
public:
	RefPtData(void);
	virtual ~RefPtData(void);
};

END_CUTLEADER_NAMESPACE()
