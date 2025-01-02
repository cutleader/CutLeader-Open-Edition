#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(EllipsePattern)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(LinePattern)
DECLARE_CUTLEADER_CLASS(ArcPattern)
DECLARE_CUTLEADER_CLASS(SingleDPattern)
DECLARE_CUTLEADER_CLASS(RoundCornerHolePattern)
DECLARE_CUTLEADER_CLASS(RadSlotPattern)
DECLARE_CUTLEADER_CLASS(TriangleHolePattern)
DECLARE_CUTLEADER_CLASS(RectHolePattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will convert some loops/patterns to standard pattern.
class ClGeometryFeature_Export PatternGrouper
{
public:
	// notes: 
	//   1) if some loops can be converted to standard pattern, we should remove that loop and the patterns in that loop, then append the std pattern and the new loop.
	//   2) the function will not use the cache data of the pattern.
	static void GroupPat(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList);

	// if return NULL, failed.
	static EllipsePatternPtr GroupEllipse(Line2DListPtr pLines);
	static SingleDPatternPtr GroupSingleD(LinePatternPtr pLinePat, ArcPatternPtr pArcPat);
	static RadSlotPatternPtr GroupRadSlot(ArcPatternPtr pArcPat1, ArcPatternPtr pArcPat2, ArcPatternPtr pArcPat3, ArcPatternPtr pArcPat4);
	static RectHolePatternPtr GroupRectHole(Line2DListPtr pLines);
	static IPatternPtr GroupOBAndDoubleD(ArcPatternPtr pArcPat1, LinePatternPtr pLinePat1, ArcPatternPtr pArcPat2, LinePatternPtr pLinePat2);
	static TriangleHolePatternPtr GroupTriangle(LinePatternPtr pLinePat1, LinePatternPtr pLinePat2, LinePatternPtr pLinePat3);
	static RoundCornerHolePatternPtr GroupRCHole(LinePatternPtr pLinePat1, ArcPatternPtr pArcPat1, LinePatternPtr pLinePat2, ArcPatternPtr pArcPat2,
										LinePatternPtr pLinePat3, ArcPatternPtr pArcPat3, LinePatternPtr pLinePat4, ArcPatternPtr pArcPat4);
};

END_CUTLEADER_NAMESPACE()
