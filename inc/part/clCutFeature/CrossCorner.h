#pragma once

#include "CornerBase.h"

DECLARE_CUTLEADER_CLASS(CrossCorner)

BEGIN_CUTLEADER_NAMESPACE()

// 交叉类型的角特征。
class ClCutFeature_Export CrossCorner : public CornerBase
{
public:
	CrossCorner(void);
	CrossCorner(double dExtVal);
	virtual ~CrossCorner(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement ICorner interface
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const override;

public: // get/set functions.
	void SetExtVal(double dVal) { m_dExtVal = dVal; }
	double GetExtVal() const { return m_dExtVal; }

private:
	// the extend val.
	double m_dExtVal;
};

END_CUTLEADER_NAMESPACE()
