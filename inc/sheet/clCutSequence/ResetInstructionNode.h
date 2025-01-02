#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(ResetInstructionNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export ResetInstructionNode : public CutNodeBase
{
public:
	ResetInstructionNode(void);
	virtual ~ResetInstructionNode(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_Reset; }
	virtual BOOL CanDraw() const { return FALSE; }
};

END_CUTLEADER_NAMESPACE()
