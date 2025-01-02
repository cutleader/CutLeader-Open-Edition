#pragma once

#include "clGeometryBaseCommon.h"
#include "LineArc2DBase.h"
#include "boost/enable_shared_from_this.hpp"
#include <set>
#include "baseConst.h"

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Arc2D)

BEGIN_CUTLEADER_NAMESPACE()


// we have four relations between two coincide lines.
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
typedef enum tagLineCoincideType
{
	LINE_COINCIDE_1			= 0,
	LINE_COINCIDE_2			= 1,
	LINE_COINCIDE_3			= 2,
	LINE_COINCIDE_4			= 3,
} LINE_COINCIDE_TYPE;


// 2d line, 二维线段。
class ClGeometryBase_Export Line2D : public LineArc2DBase, public boost::enable_shared_from_this<Line2D>
{
public:
	Line2D(void)
	{
	}

	Line2D(const Line2D& line)
	{
		m_basePt = line.GetBasePoint();
		m_baseVector = line.GetBaseVector();
		UpdateCache();
	}

	Line2D(const Point2D& basePt, const Vector2D& baseVector)
	{
		m_basePt = basePt;
		m_baseVector = baseVector;
		UpdateCache();
	}

	Line2D(const Point2D& basePt, const Point2D& endPt)
	{
		m_basePt = basePt;
		m_baseVector.SetVector(basePt, endPt);
		m_endPt = endPt;
	}

	~Line2D(void)
	{
	}

public: // implement ILineArc2D interface
	virtual const Point2D& GetStartPoint() const { return m_basePt; }
	virtual void SetStartPoint(const Point2D& pt)
	{ 
		m_basePt = pt;
		UpdateCache();
	}
	virtual const Point2D& GetEndPoint() const { return m_endPt; }
	virtual void SetEndPoint(const Point2D& newVal)
	{
		m_baseVector.SetVector(m_basePt, newVal);
		m_endPt = newVal;
	}

	virtual Point2D GetClosestPt(const Point2D& pt) const;
	virtual Point2DListPtr GetCrossPt(ILineArc2DPtr pGeom2DBase, double dTol) const;
	virtual Point2D GetPointByPercent(double dPercent) const;
	virtual Point2D MoveForward(const Point2D& fromPt, double dDistance) const;

	virtual void Transform(const Matrix2D& mat) override;
	virtual void Reverse();

	virtual double Distance(const Point2D& pt) const;
	virtual LineArc2DType GetType() const { return LineArc2D_Line; }
	virtual Rect2D GetRectBox() const;
	virtual double GetPercentByPoint(const Point2D& pt) const;
	virtual ILineArc2DPtr Clone() const;
	virtual void UpdateCache()
	{
		m_endPt = m_basePt + m_baseVector;
	}

public: // other functions.
	/************************************************************************/
	// basic props of line.

	double GetLength() const { return m_baseVector.Length(); }
	double SqrLength() const { return m_baseVector.SqrLength(); }
	void SetLength(double dLength);

	double GetBaseAngle() const { return m_baseVector.Angle(); }
	void SetBaseAngle(double dAngle);

	double GetDX() const
	{	
		double dLen = GetLength();
		const Point2D& startPt = GetBasePoint();
		const Point2D& endPt = GetEndPoint();
		double dDX = endPt.X() - startPt.X();

		return dDX;
	}
	double GetDY() const
	{
		const Point2D& startPt = GetBasePoint();
		const Point2D& endPt = GetEndPoint();
		double dDY = endPt.Y() - startPt.Y();

		return dDY;
	}
	/************************************************************************/


	/************************************************************************/
	// about crossing with other line.
	
	// get cross point between lines.
	Point2DListPtr GetLineCrossPoint(const Line2D* linePtr, double dTol = GEOM_TOLERANCE) const;

	// if this line crosses with pLine, return the percent(dPer1 and dPer2) of the cross point relative to
	// this line and pLine, and return TRUE. if they don't cross, the function will return FALSE.
	BOOL LineCrossPercent(const Line2D* pLine, double& dPer1, double& dPer2) const
	{
		/************************************************************************/
		// prepare some data.

		const Point2D& startPt1 = GetBasePoint();
		const Point2D& endPt1 = GetEndPoint();
		double dX1 = endPt1.X() - startPt1.X();
		double dY1 = endPt1.Y() - startPt1.Y();

		const Point2D& startPt2 = pLine->GetBasePoint();
		const Point2D& endPt2 = pLine->GetEndPoint();
		double dX2 = endPt2.X() - startPt2.X();
		double dY2 = endPt2.Y() - startPt2.Y();
		/************************************************************************/

		double dMat = dX1 * dY2 - dY1 * dX2;
		if (fabs(dMat) > SYSTEM_TOLERANCE)
		{
			dPer1 = (dY2 * (startPt2.X() - startPt1.X()) + dX2 * (startPt1.Y() - startPt2.Y())) / dMat;
			dPer2 = (dY1 * (startPt2.X() - startPt1.X()) + dX1 * (startPt1.Y() - startPt2.Y())) / dMat;
			return TRUE;
		}
		else
		{        
			dPer1 = dPer2 = -1;
			return FALSE;
		}
	}

	// notes:
	//   1) inner-cross means that the cross points are not at end points.
	BOOL InnerCrossWithLine(const Line2D& line) const;

	BOOL CrossWithLine(const Line2D& line) const;
	/************************************************************************/


	/************************************************************************/
	// get the angles of this line and the other geom.
	// notes: 
	//   1) we should return the angle referring to the cross point.
	//   2) the angle is [-PI, PI].

	// the angles of two lines.
	// notes: 
	//   1) actually we know the angles of two lines, but here, we should adjust the angle, if "bTowardCrossPt" is TRUE, we adjust two angles 
	//      which make its line go toward the cross point, that means we may deduct "PI" from line angle.
	void GetTwoLineAngle(Line2DPtr linePtr, Point2D crossPt, BOOL bTowardCrossPt, double& dFirstAngle, double& dSecondAngle);

	// the angles of line and arc.
	void GetLineArcAngle(Arc2DPtr arcPtr, Point2D crossPt, BOOL bTowardCrossPt, double& dFirstAngle, double& dSecondAngle);
	/************************************************************************/


	/************************************************************************/
	// about the percent.

	// cut this line using the "percent" list.
	Line2DListPtr CutLineWithPercent(const std::set<double>& perList) const;

	Line2DPtr GetPercentSegment(double dPercent1, double dPercent2) const;

	// get the sub-line from the start to "dPercent".
	Line2DPtr GetStartSegment(double dPercent) const;

	// get the sub-line from the "dPercent" to end.
	Line2DPtr GetEndSegment(double dPercent) const;

	// 得到点在线上的百分比。
	// 注：
	//  1) 还得到点到最近端点的距离。
	double GetPercentByPtEx(const Point2D& pt, double& dDis) const;
	/************************************************************************/


	/************************************************************************/
	// with points.

	// return 1 if the TestPt is on the left of this Line ,
	// return -1 if on the right, else return 0.
	int CheckSide(const Point2D& TestPt, double dTol = SYSTEM_TOLERANCE) const
	{
		const Point2D& startPt1 = GetBasePoint();
		const Point2D& endPt1 = GetEndPoint();
		double dX1 = endPt1.X() - startPt1.X();
		double dY1 = endPt1.Y() - startPt1.Y();
		double dS = dX1 * (TestPt.Y() - startPt1.Y()) - dY1 * (TestPt.X() - startPt1.X());

		double dLen = dX1*dX1 + dY1*dY1;
		if (dLen > 1)
			dS /= dLen;

		if (dS > dTol)
			return 1;
		else if(dS < -dTol)
			return -1;
		else
			return 0;
	}

	// this function is different with "Distance" function, the line is considered as infinite.
	double GetDistance(const Point2D& pt) const;

	// same with Distance function but the distance is square.
	double Distance_square(const Point2D& pt) const;

	// get the projection from the point to line.
	// notes: 
	//   1) the line is considered as infinite line.
	Point2D GetVerticalPoint(const Point2D& pt) const;

	// whether the projection point of "pt" is on the current line.
	BOOL ProjectionOnLine(const Point2D& pt) const;

	// split current line at "pt".
	// notes:
	//   1) caller should manage the memory of "pLine2Ds".
	//   2) 当分割点在端点时，分割到长度为0的线段需要剔除。
	void SplitLine(const Point2D& pt, Line2DListPtr pLine2Ds) const;
	void SplitLine(const Point2D& pt1, const Point2D& pt2, Line2DListPtr pLine2Ds) const;
	/************************************************************************/


	/************************************************************************/
	// with other lines.

	// 合并两条直线。
	// 注: 
	//   1) if cannot merge them, return FALSE. If can merge, change the current line.
	BOOL MergeWith(Line2DPtr pLine, double dParallelDis);

	// 是否平行于另一直线。
	// 注：
	//  1) 用向量叉乘也可以，不过这个函数更宽松一些。
	BOOL ParallelTo(Line2DPtr pLine) const;

	// 判断2根线段是否重合。
	// 注:
	//   1) 判断标准：
	//      a) 两条线段必须平行，且间距小于“dDis”。
	//      b) 两条线段有重叠的部分。
	BOOL CoincideWith(const Line2D* pLine, double dDis = 0) const;

	// connect two lines.
	// notes:
	//  we can always connect two lines except for they are parallel.
	/*
	                                                                 
                                                                 
				  8    8                                             
				   8 88                                              
					88                             88                
				  88  8                          88  8               
				 8     8                        8     8              
		line1 88       8  line2       line1  88       8  line2       
			  8          8                   8          8            
			88            88               88            88          
		   8                8             8                8         
		 88                  8          88                  8        
	*/
	void ConnectWith(Line2DPtr linePtr);

	// notes: use the x-coordinate to compare.
	BOOL SmallerThan(Line2DPtr pLine) const
	{
		const Point2D& startPt = GetBasePoint();
		const Point2D& endPt = GetEndPoint();
		double dDX = endPt.X() - startPt.X();

		const Point2D& startPt1 = pLine->GetBasePoint();
		const Point2D& endPt1 = pLine->GetEndPoint();
		double dDX1 = endPt1.X() - startPt1.X();

		return (dDX < 0 ? endPt : startPt) < (dDX1 < 0 ? endPt1 : startPt1);
	}

	// get the coincide type between this line and "pLine".
	// notes:
	//   1) before call this, caller must make sure these two lines are coincide.
	LINE_COINCIDE_TYPE GetCoincideType(const Line2D* pLine);

	// split current line and "pLine" by the "overlapped" region.
	/* params:
	*    1) coincideType: the coincide type of these two lines.
	*    2) pLine2Ds*: we will split lines to this variable, caller should allocate the memory.
	*    3) iComLineIdx*: flag the common line in "pLine2Ds*".
	*/
	// notes:
	//   1) two lines will be splitted like this illustration.
	/*         
				   *                     *           
				   *                     *           
				   *                     *           
				   *               line2 *           
			 |	   *              _____  *           
			/|\	   *   *                 *   *       
			 |	   *   *    |      line1 *   *       
				   *   *   \|/           *   *   
				   *   *    |            *   *       
				   *   *                 *   *   line1    
				   *   *                 *   *  _____     
					   *                     *       
					   *                     *   line2
					   *                     *       
					   *                     *       
					   *                     *       
					   *                     *       

	*/
	//    2) before call this, make sure they are coincide and get the coincide type of these two lines.
	void SplitLineByOverLap(Line2DPtr pLine, LINE_COINCIDE_TYPE coincideType, Line2DListPtr pLine2Ds1, int& iComLineIdx1, Line2DListPtr pLine2Ds2, int& iComLineIdx2);
	/************************************************************************/


	// move the line toward its left side.
	void MoveToLeft(double dDis);
};

END_CUTLEADER_NAMESPACE()
