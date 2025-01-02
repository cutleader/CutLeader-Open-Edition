#pragma once

#include "PatternBase.h"
#include "IArrayPattern.h"

DECLARE_CUTLEADER_CLASS(ArrayPatternBase)

BEGIN_CUTLEADER_NAMESPACE()

// this class works as the basic class for the grid type pattern.
// notes: 
//   1) the "m_basePt" is the base point of the grid pattern.
class ClGeometryFeature_Export ArrayPatternBase : public PatternBase,
										public IArrayPattern
{
public:
	ArrayPatternBase(void);
	virtual ~ArrayPatternBase(void);

public: // implement IPattern interface.
	virtual BOOL IsSmooth() const override { return GetControlPattern()->IsSmooth(); }
	virtual BOOL IsArrayPattern() const override { return TRUE; }
	virtual Point2D GetPointByPatPos(const PatternPosition& patternPosition) const override; // get the point on the first inst.
	virtual PatternPosition GetPatPosByPoint(const Point2D& pt, PatternPosition& secondPatPos) const override; // "pt" should on the first inst.

public: // implement IArrayPattern interface.
	IPatternPtr GetControlPattern() const override;
	void SetControlPattern(IPatternPtr pPattern) override;
	virtual BOOL OffsetArrayPattern(BOOL bInside, double dOffsetVal) override;
};

END_CUTLEADER_NAMESPACE()
