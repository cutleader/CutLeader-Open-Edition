#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(LinePattern)
DECLARE_CUTLEADER_CLASS(ArcPattern)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(EllipsePattern)
DECLARE_CUTLEADER_CLASS(CirclePattern)
DECLARE_CUTLEADER_CLASS(RectHolePattern)
DECLARE_CUTLEADER_CLASS(RoundCornerHolePattern)
DECLARE_CUTLEADER_CLASS(SingleDPattern)
DECLARE_CUTLEADER_CLASS(DoubleDPattern)
DECLARE_CUTLEADER_CLASS(LineArrayPattern)
DECLARE_CUTLEADER_CLASS(ArcArrayPattern)
DECLARE_CUTLEADER_CLASS(CircleArrayPattern)
DECLARE_CUTLEADER_CLASS(GridArrayPattern)
DECLARE_CUTLEADER_CLASS(TriangleHolePattern)
DECLARE_CUTLEADER_CLASS(OBroundPattern)
DECLARE_CUTLEADER_CLASS(RadSlotPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the common interface for all visitors to pattern.
class ClGeometryFeature_Export IPatternVisitor
{
public:
	virtual BOOL Visit(LinePatternPtr pLinePat) = 0;
	virtual BOOL Visit(ArcPatternPtr pArcPat) = 0;
    virtual BOOL Visit(PolygonPatternPtr pPolygonPat) = 0;
	virtual BOOL Visit(EllipsePatternPtr pEllipsePat) = 0;
	virtual BOOL Visit(CirclePatternPtr pCirclePat) = 0;
	virtual BOOL Visit(RectHolePatternPtr pRectHolePat) = 0;
	virtual BOOL Visit(RoundCornerHolePatternPtr pRCHolePat) = 0;
	virtual BOOL Visit(SingleDPatternPtr pSingleDPat) = 0;
	virtual BOOL Visit(DoubleDPatternPtr pDoubleDPat) = 0;
	virtual BOOL Visit(LineArrayPatternPtr pPatLinePat) = 0;
	virtual BOOL Visit(ArcArrayPatternPtr pPatArcPat) = 0;
	virtual BOOL Visit(CircleArrayPatternPtr pPatCirclePat) = 0;
	virtual BOOL Visit(GridArrayPatternPtr pPatGridPat) = 0;
	virtual BOOL Visit(TriangleHolePatternPtr pTrHolePat) = 0;
	virtual BOOL Visit(OBroundPatternPtr pOBroundPat) = 0;
	virtual BOOL Visit(RadSlotPatternPtr pRadSlotPat) = 0;
};

END_CUTLEADER_NAMESPACE()