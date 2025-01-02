#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(CoolingPointNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export CoolingPointNode : public CutNodeBase
{
public:
	CoolingPointNode(void);
	virtual ~CoolingPointNode(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_CoolingPoint; }
	virtual BOOL CanDraw() const { return FALSE; }
};

END_CUTLEADER_NAMESPACE()
