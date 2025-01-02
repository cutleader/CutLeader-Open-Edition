#pragma once

#include "CornerBase.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(CoolingPointCorner)

BEGIN_CUTLEADER_NAMESPACE()

// È¦ÐÎ×´µÄ½ÇÌØÕ÷¡£
class ClCutFeature_Export CoolingPointCorner : public CornerBase
{
public:
	CoolingPointCorner(void);
	virtual ~CoolingPointCorner(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement ICorner interface
    virtual bool HasCutPath() const override { return false; }
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const override;
    virtual bool HasCornerValue() const override { return false; }
};

END_CUTLEADER_NAMESPACE()
