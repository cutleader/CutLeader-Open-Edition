#pragma once

#include "CornerBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(RadiusCorner)

BEGIN_CUTLEADER_NAMESPACE()

// 圆弧类型的角特征。
class ClCutFeature_Export RadiusCorner : public CornerBase
{
public:
	RadiusCorner(void);
	RadiusCorner(double dCornerRad);
	virtual ~RadiusCorner(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement ICorner interface
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const override;

public: // set/get functions.
	void SetCornerRadius(double dCornerRad) { m_dCornerRad = dCornerRad; }
	double GetCornerRadius() const { return m_dCornerRad; }

private:
	/************************************************************************/
	// helper functions to get the arc at the corner.
	// notes: 
	//   1) "pPat1","pPat2" do not follow the cut order.
	//   2) we should adjust the result to follow the cut direction.

	// get the arc between two line patterns.
	// notes: the arc returned will follow the cut direction.
	LineArc2DListPtr BuildTwoLineCorner(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection, IPatternPtr pPat1, IPatternPtr pPat2) const;

	// get the arc between line and arc pattern.
	// notes: the arc returned will follow the cut direction.
	LineArc2DListPtr BuildLineArcCorner(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection, IPatternPtr pPat1, IPatternPtr pPat2) const;

	// get the arc between two arc patterns.
	// notes: the arc returned will follow the cut direction.
	LineArc2DListPtr BuildTwoArcCorner(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection, IPatternPtr pPat1, IPatternPtr pPat2) const;
	/************************************************************************/

private:
	// the corner radius
	double m_dCornerRad;
};

END_CUTLEADER_NAMESPACE()
