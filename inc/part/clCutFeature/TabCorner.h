#pragma once

#include "CornerBase.h"

DECLARE_CUTLEADER_CLASS(TabCorner)

BEGIN_CUTLEADER_NAMESPACE()

// the tab corner feature.
class ClCutFeature_Export TabCorner : public CornerBase
{
public:
	TabCorner(void);
	TabCorner(double dTabVal);
	virtual ~TabCorner(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement ICorner interface
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const override;

public: // get/set functions.
	void SetTabValue(double dVal) { m_dTabVal = dVal; }
	double GetTabValue() const { return m_dTabVal; }

private:
	double m_dTabVal;
};

END_CUTLEADER_NAMESPACE()
