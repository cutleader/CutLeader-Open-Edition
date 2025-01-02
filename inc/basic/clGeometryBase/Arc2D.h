#pragma once

#include "clGeometryBaseCommon.h"
#include "LineArc2DBase.h"
#include "boost/enable_shared_from_this.hpp"
#include <set>

DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(Line2D)

BEGIN_CUTLEADER_NAMESPACE()


class Arc2D;
typedef boost::shared_ptr<std::vector<Arc2DPtr>> Arc2DsPtr;


// 圆弧的四种重叠类型，把示意图中的直线想成圆弧。
/*  
						 *                     
						 *                     
						 *       *     *       
	   * *     *       * *       *     *       
	   * *     *       * *     * *     * *     
	   * *     *       * *     * *     * *     
	   * *     * *     * *     * *     * *     
	   * *     * *     *       * *     * *     
	   * *     * *     *       * *     * *     
	   * *     * *             * *     * *     
				 *               *     *       
				 *               *     *       
				 *               *     *       
			   ____________
        1)          2)          3)      4)
*/
enum ARC_COINCIDE_TYPE
{
	ARC_COINCIDE_1			= 0,
	ARC_COINCIDE_2			= 1,
	ARC_COINCIDE_3			= 2,
	ARC_COINCIDE_4			= 3,
};


// 2d arc, 二维圆弧。
class ClGeometryBase_Export Arc2D : public LineArc2DBase, public boost::enable_shared_from_this<Arc2D>
{
public:
	Arc2D(void);
	Arc2D(const Point2D& centerPt, double dRadius, double dStartAngle, double dMoveAngle);
	~Arc2D(void);

public: // implement ILineArc2D interface
	virtual const Point2D& GetStartPoint() const { return m_startPt; }
	virtual void SetStartPoint(const Point2D& pt); // this function change the start point, but keeps sweep angle and the end point. 
	virtual const Point2D& GetEndPoint() const { return m_endPt; }
	virtual void SetEndPoint(const Point2D& pt); // this function change the end point, but keeps sweep angle and the start point. 

	// 得到圆弧上距离pt最近的点。
	// 注：
	// 1) before call this, make sure sweep angle of this arc is bigger than GEOM_TOLERANCE.
	// 2) 待完善：当圆弧半径很大，比如10万左右时，靠近圆弧两个端点的点（在圆弧外面）会被认为在扇形区域内，会算出不对的最近点。
	virtual Point2D GetClosestPt(const Point2D& pt) const;

	virtual Point2DListPtr GetCrossPt(ILineArc2DPtr pGeom2DBase, double dTol) const;
	virtual Point2D GetPointByPercent(double dPercent) const;
	virtual Point2D MoveForward(const Point2D& fromPt, double dDistance) const;

	virtual void Transform(const Matrix2D& mat) override;
	virtual void Reverse();

	virtual double Distance(const Point2D& pt) const;
	virtual LineArc2DType GetType() const { return LineArc2D_Arc; }
	virtual Rect2D GetRectBox() const;
	virtual double GetPercentByPoint(const Point2D& pt) const;
	virtual ILineArc2DPtr Clone() const;
	virtual void UpdateCache();

public: // set/get functions.
	Vector2D GetMidVector() const { return m_midVector; }
	void SetMidVector(const Vector2D& newVal) { m_midVector = newVal; }

	// whether this arc is a circle.
	BOOL IsCircle() const { return m_bCircle; }

public:
	/************************************************************************/
	// 圆弧的一些基本属性。

	// change the base angle.
	// notes:
	//   1) the sweep angle not change.
	void SetBaseAngle(double dAngle);

	double GetBaseAngle() const { return m_baseVector.Angle(); }

	double GetRadius() const { return m_baseVector.Length(); }
	void SetRadius(double dRadius);

	double GetDiameter() const { return GetRadius()*2; }

	double GetStartAngle() const { return m_baseVector.Angle(); }

	// get the sweep angle.
	// notes:
	//   1) for circle, TWOPI means CCW, -TWOPI means CW.
	double GetSweepAngle() const;

	// set the sweep angle.
	// notes:
	//   1) for circle, TWOPI means CCW, -TWOPI means CW.
	void SetSweepAngle(double dAngle);

	// get the length of the chord.
	double GetChordLen() const { return fabs(2 * m_baseVector.Length() * sin(GetSweepAngle()/2)); }

	// 得到弓的高度
	double GetBowHeight() const;

	// get the length of the arc.
	double GetArcLen() const { return m_baseVector.Length()*fabs(GetSweepAngle()); }

	// get the area of the arc.
	double GetArcArea() const;

	// 得到中点
	Point2D GetMiddlePoint() const;
	/************************************************************************/


	/************************************************************************/
	// 圆弧和其他图形的交点。

	// get the cross point with the line.
	// notes:
	//   1) before call this, make sure length of "linePtr" is bigger than "dTol".
	Point2DListPtr GetLineCrossPoint(const Line2D* linePtr, double dTol = GEOM_TOLERANCE) const;

	// get the cross point with the line.
	// notes:
	//   1) before call this, make sure length of "linePtr" is bigger than "dTol".
	//   2) this function will consider "linePtr" is infinite.
	Point2DListPtr GetLineCrossPointEx(Line2DPtr linePtr, double dTol = GEOM_TOLERANCE) const;

	// get the cross point with the arc.
	Point2DListPtr GetArcCrossPoint(Arc2DPtr arcPtr, double dTol = GEOM_TOLERANCE) const;
	/************************************************************************/


	/************************************************************************/
	// 关于转化为直线。

	// if the sweep angle is "very" small, we think the arc is "like" a line.
	BOOL LikeLine() const;

	// this angle is special, it indicates how the arc like a line.
	double GetArcAngle() const;

	// return the segment num if we want to explode the arc to lines.
	// notes:
	//   1) dFactor: this will decide the number. be smaller, more lines can be gotten.
	int ExplodeLineNum(double dFactor) const;
	/************************************************************************/


	/************************************************************************/
	// 和其他图形的角度。

	// get the tangent arc between this arc and "arcPtr"
	// if return FALSE, that means we cannot find such arc.
	/* params:
	*  radius: the radius of the arc we want.
	*  arc1Pt,arc2Pt: two tangent point.
	*  centerPt: the center point of the arc we want.
	*/
	BOOL GetTangArcWith(Arc2DPtr arcPtr, double dRadius, Point2D& arc1Pt, Point2D& arc2Pt, Point2D& centerPt) const;

	// get the tangent arc between this arc and "linePtr", if return FALSE, that means we cannot find such arc.
	/* params:
	*  connPt: the cross point between this arc and "linePtr"
	*  dRadius: the radius of the arc we want.
	*  linePt: the tangent point at "linePtr".
	*  arcPt: the tangent point at this arc.
	*  centerPt: the center point of the arc we want.
	*/
	BOOL GetTangArcWith(Line2DPtr linePtr, const Point2D& connPt, double dRadius, Point2D& linePt, Point2D& arcPt, Point2D& centerPt) const;

	// please refer to the similar method in "Line2D" class.
	void GetTwoArcAngle(Arc2DPtr arcPtr, const Point2D& crossPt, BOOL bTowardCrossPt, double& dFirstAngle, double& dSecondAngle) const;
	/************************************************************************/


	/************************************************************************/
	// about the percent.

	// cut this arc using the "percent" list.
	// 注：
	//   1) 弧长小于“GEOM_TOLERANCE”的圆弧被抛弃。
	Arc2DsPtr CutArcWithPercent(const std::set<double>& perList);

	Arc2DPtr GetPercentSegment(double dPercent1, double dPercent2) const;

	// get the sub-arc from the start to "dPercent".
	Arc2DPtr GetStartSegment(double dPercent) const;

	// get the sub-arc from the "dPercent" to end.
	Arc2DPtr GetEndSegment(double dPercent) const;
	/************************************************************************/


	// get the sweep angle from start point to pt.
	// notes:
	//   1) the returned angle has the same direction with the sweep angle of the arc.
	//   2) for circle, if "pt" is on the start point, this function return 0.
	double GetMoveAngleAtPt(const Point2D& pt) const;

	// get the sweep angle from startPt to endPt.
	// notes: 
	//   1) the returned angle has the same direction with the sweep angle of the arc.
	//   2) startPt and endPt should follow the arc direction. otherwise the returned angle will not have the same direction with the sweep angle of the arc.
	//   3) for circle, if "startPt" and "endPt" are both on the start point, this function return 0.
	double GetMoveAngleByPt(const Point2D& startPt, const Point2D& endPt) const;

	// merge this arc with "arcPtr".
	// notes: 
	//   1) if cannot merge them, return FALSE.
	//   2) if two arcs have same center point and radius and there are some overlap, we can merge them, update this.
	//   3) if two arcs consist a very oblate moon, drop arcPtr.
	BOOL MergeWith(Arc2DPtr arcPtr);

	// 判断2根圆弧是否重合。
	// 注:
	//   1) 判断标准：
	//      a) 两条圆弧必须平行(同心圆)，且间距小于“dDis”。
	//      b) 两条圆弧有重叠的部分。
	BOOL CoincideWith(Arc2DPtr pArc2D, double dDis = 0) const;

	// 得到2个圆弧的重叠类型。
	// 注:
	//   1) 调用者必须确定2个圆弧重叠。
	//   2) 待完善：好像忽略了一个重叠类型，两个圆弧组成了整圆，且过头的情况。
	ARC_COINCIDE_TYPE GetCoincideType(Arc2DPtr pArc2D) const;

	// 基于重叠区域分割这2个圆弧。
	/* 参数:
	*    1) coincideType: the coincide type of these two lines.
	*    2) pArc2Ds*: we will split lines to this variable, caller should allocate the memory.
	*    3) iComLineIdx*: flag the common line in "pArc2Ds*".
	*/
	// 注:
	//   1) 调用者必须确定2个圆弧重叠。
	void SplitArcByOverLap(Arc2DPtr pArc2D, ARC_COINCIDE_TYPE coincideType, Arc2DsPtr pArc2Ds1, int& iComArcIdx1, Arc2DsPtr pArc2Ds2, int& iComArcIdx2);

	// 计算圆弧到点的距离。
	// 注：
	// 1) 为了性能考虑设计了这个函数，如需计算一个圆弧到许多点的距离，那么把一些不变的值先算好传进来。
	double SquareDistance_2(const Point2D& pt, double dSweepAngle, double dRadius) const;

protected:
	// the vector from the center point to the mid-point of the arc.
	Vector2D m_midVector;

	// whether is circle.
	BOOL m_bCircle;

	// the direction of the circle.
	DIRECTION m_circleDir;

	// buffer the start pt.
	Point2D m_startPt;
};

END_CUTLEADER_NAMESPACE()
