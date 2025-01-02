#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "ClData.h"
#include "SubjectBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// “回路角特征”信息。
// 注: 
//   1) "SinglePatClosedPath" and "GridPatternLoop" loop do not have loop corner data. BUT "LoopCutFeature" will still keep a loop corner data of such loop,
//      even though it(m_pLoopCorner) is NULL.
class ClCutFeature_Export LoopCornerData : public ClData,
									 public SubjectBase
{
public:
	LoopCornerData(void);
	LoopCornerData(IPatternLoopPtr patLoopPtr);
	virtual ~LoopCornerData(void);

public: // implement IData interface
	// notes:
	//   1) do not clone pattern loop.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

	void SetCornerList(CornerListPtr pCornerList) { m_pCornerList = pCornerList; }
	CornerListPtr GetCornerList() const { return m_pCornerList; }

public: // other functions.
	// get all line/arc shapes of the corners.
	virtual LineArc2DListPtr Calculate2DLineArcs(BOOL bFollowLoopDirection) const;

	// get all Corner points in this loop.
	// notes: actually it's the cross point of the two patterns.
	Point2DListPtr GetAllCornerPts() const;

	// get the Corner feature at point: pt.
	// notes: may return NULL.
	ICornerPtr GetCornerAtLoc(const Point2D& pt) const;

	// get the corner between two pattern.
	// notes: may return NULL.
	ICornerPtr GetCornerByPat(LONGLONG iPatID1, LONGLONG iPatID2) const;

	// 得到所有“角特征”和“角特征点”配对。
	std::vector<std::pair<ICornerPtr, Point2D>> GetAllCornerPtPairs() const;

	// 得到所有角特征所在的“图形对”。
	std::vector<std::pair<LONGLONG, LONGLONG>> GetAllCornerPats() const;

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

	// the corner list
	CornerListPtr m_pCornerList;
};

END_CUTLEADER_NAMESPACE()
