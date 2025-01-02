#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include "PatternLoopBase.h"

DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(PolygonPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the polygon pattern loop.
// 注:
//   1) control point and connect point, these concepts are for this loop.
//      control point is start/end pt of pattern node, and around connect point, there must be two adjacent pattern nodes.
//   2) loop nodes in the loop follows the loop direction, we can figure out the loop direction by the nodes.
//   3) 多边形轮廓中要不只有一个polygon pattern，要不都是line/arc pattern。
//   4) 对于多边形轮廓中只有一个polygon pattern时，很多成员函数没有实现，不过目前用不到。
class ClGeometryFeature_Export PolygonPatternLoop : public PatternLoopBase
{
public:
	PolygonPatternLoop();
	virtual ~PolygonPatternLoop();

public: // implement IData interface.
	virtual IDataPtr Clone() const override; // do not clone the pattern within the loop node.

public: // implement IPatternLoop interface.
	virtual void ReverseDirection() override;
	virtual DIRECTION GetDirection() const override;
	virtual Vector2D GetDirectionOnPt(LONGLONG iPatternID, const PatternPosition& patternPosition) const override;
	virtual PatternLoopType GetPatternLoopType() const override { return PatternLoop_Polygon; }
	virtual BOOL IsOpenPath(double dTol) const override;

public:
	/************************************************************************/
	// about add/remove pattern to/from the loop.

	// add pattern directly to the begin/end of this loop.
	// notes: 
	//   1) we should indicate whether the pattern has the same direction with the loop.
	//   2) when we load loops from database, we will call this function to add loop node.
	BOOL AppendPattern(const IPatternPtr pPattern, BOOL bLoopDirection);
	BOOL InsertPattern(const IPatternPtr pPattern, BOOL bLoopDirection);

	// add pattern to the open loop.
	// notes:
	//   1) the open loop only includes line/arc patterns, so "pPattern" will be line/arc pattern.
	BOOL AddPattern(IPatternPtr pPattern, double dTol = SYSTEM_TOLERANCE);

	// delete a pattern directly from the loop.
	void RemovePattern(IPatternPtr pPattern);
	/************************************************************************/


	/************************************************************************/
	// the relations between pattern and loop.

	// check whether start or end point of the open path CAN connect with the start or end point of "pPattern".
	/* params:
	*  dTol: use this to check the equation.
	*/
	// notes: 
	//   1) you know, for close path, we should not call this.
	//   2) "pPattern" is not "closed" pattern.
	BOOL CanLinkWithPat(const IPattern* pPattern, double dTol = SYSTEM_TOLERANCE) const;

	// whether "pPattern" stay at two ends of the open loop. 
	// notes: 
	//   1) call this for open path.
	BOOL PatAtOpenLoopEnd(IPatternPtr pPattern);
	/************************************************************************/


	/************************************************************************/
	// about adjacent patterns.

	// get the two patterns will meet at the connection point.
	// notes:
	//   1) if the loop only has one base geom, do not call this.
	//   2) for open path, "iConnectPtIdx" should not be the first or last connection point.
	//   3) "iPatID1" and "iPatID2" follow the loop direction.
	BOOL GetTwoPatternsAtConnectionPt(int iConnectPtIdx, LONGLONG& iPatID1, LONGLONG& iPatID2) const;

	// get two base geoms which meet at the connection point.
	// notes:
	//   1) if the loop only has one base geom, do not call this.
	//   2) for open path, "connectPt" should not be the first or last connection point.
	//   3) two base geoms follow the loop direction.
	LineArc2DListPtr GetTwoGeomAtConPt(Point2D connectPt);

	// get the angle of the two pattern which meet at "iConnectPtIdx".
	// 注:
	// 1) this function is for close path.
	// 2) the angle is "inside" the pattern loop.
	double GetTwoPatternInsideAngle(int iConnectPtIdx) const;
	double GetTwoPatternInsideAngle_2(int iConnectPtIdx, DIRECTION emDirection) const; // 该函数事先计算好轮廓的方向，在大量调用该接口时该函数有性能优势。

	// get the angle of the two pattern which meet at "iConnectPtIdx".
	// notes:
	//   1) this function is for open path.
	//   2) the angle is at the "left" side of the open loop.
	//   3) caller should make sure "iConnectPtIdx" is corner pt, not an end pt of open path.
	double GetTwoPatLeftAng(int iConnectPtIdx);

	// get the cross point of two pattern.
	// notes: 
	//   1) before call this, caller must make sure that these two pattern is adjacent.
	Point2D GetPatCrossPt(LONGLONG iPatID1, LONGLONG iPatID2) const;
	/************************************************************************/


	/************************************************************************/
	// about control points of the loop.

	// 得到多边形轮廓的起点和终点。
	// 注：
	// 1) 这两个函数没有使用缓存数据。
	Point2D GetStartPt() const;
	Point2D GetEndPt() const;
	// 这两个函数使用了轮廓中几何特征的缓存，效率更高。
	const Point2D& GetStartPt_byRef() const;
	const Point2D& GetEndPt_byRef() const;

	// get ctrl points of the loop.
	// notes: 
	//   1) please refer to "LineArc2DLoop::GetPointList()".
	//   2) if "bConnection" is TRUE, the start/end points of open path will be dropped.
	Point2DListPtr GetCtrlPtList(BOOL bConnection = FALSE) const;
	/************************************************************************/


	// get the next pattern of "pPattern".
	// notes: 
	//   1) for close path, if "pPattern" is last one, the "next" is the first one.
	//   2) for open path, if "pPattern" is last one, the "next" will be NULL.
	IPatternPtr GetNextPat(IPatternPtr pPattern) const;

	// 得到开放回路的所有图形，它们遵守回路的方向。调用这个函数时先确认这个回路是开放的。
	LineArc2DListPtr GetPath_of_openLoop() const;

	// 把多边形轮廓转化为多边形pattern。
	// 注：
	// 1) 该函数没有更新缓存。
	PolygonPatternPtr Convert2PolygonPattern();

    // 如果多边形回路中是2个首尾相连的直线或者完全重叠的两个圆弧，返回true。
    bool IsTwoOverlappedGeom2Ds() const;

    // 该多边形几何轮廓是不是圆。
    bool IsCircle() const;

    // 该多边形几何轮廓是不是正多边形。
    bool IsRegularPolygon() const;

private:
	void SetLoopNodes(PatternLoopNodeListPtr pPatternLoopNodeList) { m_pLoopNodeList = pPatternLoopNodeList; }

	// 拆分回路的各个节点，生成一个新的回路。
	// 注：
	// 1) 该函数没有更新几何轮廓及其包含几何特征的缓存。
	PolygonPatternLoopPtr SplitAllNodes() const;

	// 拆分回路的弧度大于90度的圆弧节点为4等分节点，生成一个新的回路。
	// 注：
	// 1) 该函数没有更新几何轮廓及其包含几何特征的缓存。
	PolygonPatternLoopPtr SplitArcNodes() const;

	// 拆分“左点”的2个相邻图形中的一个，这个地方需要去看文档
	// 注：
	// 1) 该函数没有更新几何轮廓及其包含几何特征的缓存。
	PolygonPatternLoopPtr SplitOneArcGeom_of_twoAdjacentGeoms(LONGLONG iArcPatID) const;
};

END_CUTLEADER_NAMESPACE()
