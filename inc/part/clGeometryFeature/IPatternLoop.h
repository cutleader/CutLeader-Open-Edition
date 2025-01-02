#pragma once

#include "baseConst.h"
#include "Rect2D.h"
#include "IData.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopNodeList)
DECLARE_CUTLEADER_CLASS(PatternLoopCache)

BEGIN_CUTLEADER_NAMESPACE()


// we defined three pattern loop type.
enum PatternLoopType
{
	PatternLoop_Polygon			= 0, // the pattern loop is composed with line/arc patterns.
	PatternLoop_Std			    = 1, // the pattern loop is composed with one standard pattern.
	PatternLoop_Array			= 2, // the pattern loop is composed with one array pattern.
};


// the interface for pattern loop, 由几何特征连成的轮廓，简称轮廓或几何轮廓。
class ClGeometryFeature_Export IPatternLoop : virtual public IData
{
public:
	IPatternLoop();
	virtual ~IPatternLoop() {}

public:
	// 关于得到回路中的图形。
	virtual PatternListPtr GetPatternList() const = 0;
	virtual IPatternPtr GetPatternByLoopNodeIndex(int iIndex, BOOL& bLoopDirection) const = 0;
	virtual IPatternPtr GetPatternByLoopNodeIndex(int iIndex) const = 0;
	virtual IPatternPtr GetPatternByID(LONGLONG iPatternID, BOOL& bLoopDirection) const = 0;
	virtual IPatternPtr GetPatternByID(LONGLONG iPatternID) const = 0;

	// 关于得到回路节点。
	virtual int GetLoopNodeIndexByPatternID(LONGLONG iPatternID) const = 0;
	virtual PatternLoopNodeListPtr GetPatternLoopNodes() const = 0;


	/************************************************************************/
	// about the direction.

	// notes:
	//   1) for "SinglePatClosedPath" and "ArrayPatternLoop", do not think that the direction is always CCW, we should take "PatternLoopNode::m_bLoopDirection" into account also.
	//   2) for open polygon path, this is meaningless.
	//   3) for closed polygon path, calculate it.
	virtual DIRECTION GetDirection() const = 0;

	// reverse the direction of the loop.
	// notes:
	//   1) for "polygon" loop, reverse the order of the node, and also change "m_bLoopDirection" in PatternLoopNode. 
	//   2) for "one pattern" loop, we should only change "m_bLoopDirection" in PatternLoopNode.
	virtual void ReverseDirection() = 0;

	// get the direction vector on the point.
	// notes: 
	//   1) if "patternPosition" at corner, this function can cover it also.
	//   2) the returned vector was normalized.
	virtual Vector2D GetDirectionOnPt(LONGLONG iPatternID, const PatternPosition& patternPosition) const = 0;
	/************************************************************************/


	/************************************************************************/
	// get some point on the loop.

	// get the cross point info between the loop and line, we may get more than one cross point, but we only return closest one.
	// notes: 
	//   1) for "ArrayPatternLoop", we only get the cross point with the first instance of array pattern.
	/* params:
	* basePt, dAngle : these two params decide a line.
    * mat : if caller set this variable, we transform the loop using "mat", then calculate the closest cross point.
	* crossPt : the cross point.
	* iIndex : the loop node which has the cross point.
	*/
	virtual void GetClosestCrossPt(const Point2D& basePt, double dAngle, const Matrix2D& mat, Point2D& crossPt, int& iIndex) const = 0;

	// get the point on the loop, this point must be the closest point to "pt".
	/* params:
	*  pt : the reference point.
	*  closePt : the point on the loop.
	*  iIndex : the loop node which has the "closePt".
	*  mat : if caller set this variable, we transform the loop using "mat", then calculate the closest point.
	*/
	virtual void GetClosestPt(const Point2D& pt, Point2D& closePt, int& iIndex, const Matrix2D& mat) const = 0;
	/************************************************************************/


	// get the loop type.
	virtual PatternLoopType GetPatternLoopType() const = 0;

	// 检查轮廓是不是开放的，该函数没有使用缓存数据。
	virtual BOOL IsOpenPath(double dTol = GEOM_TOLERANCE) const = 0;

	// whether this loop is composed of "pPatList" or some patterns in "pPatList".
	virtual BOOL FromPatList(const PatternList* pPatList) const = 0;

public:
	// 有关缓存。
	PatternLoopCachePtr GetCacheData() const { return m_pPatternLoopCache; }
	const PatternLoopCache* GetCacheData_FR() const { return m_pPatternLoopCache.get(); }

protected:
	// the cache data of the pattern loop.
	// 注：
	// 1) 该变量放在接口中，这样可以不用虚函数来访问它，效率方面有优势。
	PatternLoopCachePtr m_pPatternLoopCache;
};

END_CUTLEADER_NAMESPACE()
