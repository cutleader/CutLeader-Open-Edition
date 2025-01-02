#pragma once

#include "CutSequenceBase.h"
#include "ICutPartSequence.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export CutPartSequenceBase : virtual public ICutPartSequence,
									 public CutSequenceBase
{
public:
	CutPartSequenceBase(void);
	virtual ~CutPartSequenceBase(void);

public: // implement interface ICutSequence.
	virtual BOOL IsCutPart() const override { return TRUE; }
    virtual bool SequenceWillBeCut() const;
};

END_CUTLEADER_NAMESPACE()
