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

// ����·�����������ϡ�
class ClCutFeature_Export LoopCornerDataList : public std::vector<LoopCornerDataPtr>,
										 public ClData
{
public:
	LoopCornerDataList(void);
	~LoopCornerDataList(void);

public:
	// get the loop Corner by the pattern loop id.
	LoopCornerDataPtr GetLoopCornerByLoop(LONGLONG iLoopId) const;

	// ���ݡ���·��������ID�õ�����·������������
	LoopCornerDataPtr GetItemByID(LONGLONG iLoopCornerID) const;

	// get all Corner points.
	Point2DListPtr GetAllCornerPts() const;

	// get the loop which hold the corner point.
	// notes:
	//   1) before call this, make sure "cornerPt" is a real corner point, otherwise return null.
	IPatternLoopPtr GetLoopByCornerPt(const Point2D& cornerPt) const;

	// �õ����С��������㡱����Ӧ�ġ�����������
	std::vector<std::pair<ICornerPtr, Point2D> > GetAllCornerPtPairs() const;
};

END_CUTLEADER_NAMESPACE()
