#pragma once

#include "CornerBase.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(RingCorner)

BEGIN_CUTLEADER_NAMESPACE()

// È¦ÐÎ×´µÄ½ÇÌØÕ÷¡£
class ClCutFeature_Export RingCorner : public CornerBase
{
public:
	RingCorner(void);
	RingCorner(double dLoopVal);
	virtual ~RingCorner(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement ICorner interface
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const override;

public: // set/get functions.
	void SetLoopValue(double dVal) { m_dLoopVal = dVal; }
	double GetLoopValue() const { return m_dLoopVal; }

private:
	double m_dLoopVal;
};

END_CUTLEADER_NAMESPACE()
