#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// “回路角特征”集合。
class ClCutFeature_Export LoopCornerDataList : public std::vector<LoopCornerDataPtr>,
										 public ClData
{
public:
	LoopCornerDataList(void);
	~LoopCornerDataList(void);

public:
	// get the loop Corner by the pattern loop id.
	LoopCornerDataPtr GetLoopCornerByLoop(LONGLONG iLoopId) const;

	// 根据“回路角特征”ID得到“回路角特征”对象。
	LoopCornerDataPtr GetItemByID(LONGLONG iLoopCornerID) const;

	// get all Corner points.
	Point2DListPtr GetAllCornerPts() const;

	// get the loop which hold the corner point.
	// notes:
	//   1) before call this, make sure "cornerPt" is a real corner point, otherwise return null.
	IPatternLoopPtr GetLoopByCornerPt(const Point2D& cornerPt) const;

	// 得到所有“角特征点”及对应的“角特征”。
	std::vector<std::pair<ICornerPtr, Point2D> > GetAllCornerPtPairs() const;
};

END_CUTLEADER_NAMESPACE()
