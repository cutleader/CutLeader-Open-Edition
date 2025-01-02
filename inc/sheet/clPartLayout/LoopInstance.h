#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Point2D.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(LoopInstanceCache)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(ICorner)


BEGIN_CUTLEADER_NAMESPACE()


// “回路实例”。
// 注:
//   1) the loop instance is different with other data object, we cannot use its ID to compare, so only part instance and the pattern loop ID can decide a loop instance.
class ClPartLayout_Export LoopInstance : public ClData
{
public:
	LoopInstance(void);
	LoopInstance(PartInstancePtr pPartInstance);
	virtual ~LoopInstance(void);

public: // implement IData interface.
	// clone the loop inst.
	// notes:
	//   1) do not clone "m_pPartInstance".
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void SetPartInstance(PartInstancePtr pPartInstance) { m_pPartInstance = pPartInstance; }
	PartInstancePtr GetPartInstance() const { return m_pPartInstance; }

	void SetPatternLoopID(LONGLONG iId) { m_iPatternLoopID = iId; }
	LONGLONG GetPatternLoopID() const { return m_iPatternLoopID; }

	void SetCacheData(LoopInstanceCachePtr pLoopInstCache) { m_pLoopInstCache = pLoopInstCache; }
	LoopInstanceCachePtr GetCacheData() const { return m_pLoopInstCache; }

public:
	/************************************************************************/
	// 关于加工工艺。

	// 得到起切点。
	// notes:
	//   1) if this LoopInstance is a "GridPatternLoop", start/end points will be on the first instance
	//      of the grid pattern. but if bArrayLastInst is TRUE, the end cut point will on the last inst.
	Point2D GetStartCutPt() const;
	Point2D GetEndCutPt(BOOL bArrayLastInst = TRUE) const;

	// whether lead in/out are enabled for this loop.
	BOOL EnableLeadIn() const;
	BOOL EnableLeadOut() const;

	// 得到回路实例上的所有微连接点。
	Point2DListPtr GetAllMicroJointPts() const;

	// 得到回路实例上所有“微连接点”对应的“微连接特征”。
	std::vector< std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs() const;

	// 得到回路实例上的所有角点。
	Point2DListPtr GetAllCornerPts();

	// 得到回路实例上所有“角点”及对应的“角特征”。
	std::vector< std::pair<ICornerPtr, Point2D> > GetAllCornerPtPairs();
	/************************************************************************/


	// get the transform matrix which can transform loop instance to sheet mode.
	Matrix2D GetTransformMatrix() const;

	// 回路实例到点的距离。
	// 注：
	//  1) 要使用缓存。
	double DistanceTo(const Point2D& pt);

	// “回路实例”上距离pt最近的点。
	Point2D GetClosestPt(const Point2D& pt);

private:
	PartInstancePtr m_pPartInstance;

	// the pattern loop id
	LONGLONG m_iPatternLoopID;

	// the cache data of the loop inst.
	LoopInstanceCachePtr m_pLoopInstCache;
};

END_CUTLEADER_NAMESPACE()
