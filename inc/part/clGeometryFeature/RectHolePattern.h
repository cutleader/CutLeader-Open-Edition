#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(RectHolePattern)

BEGIN_CUTLEADER_NAMESPACE()

// the rect hole pattern.
// notes:
//   1) we have an assumption here! That is, start point is the left top point,
//      end point is right bottom point.
/*
    variables in the picture are consistent to the variable in the program.

			 (x1,y1)         (x2,y2)
			 (startPt)	
			        (length)
				 ______________
				|			   |
				|			   |   (width)
				|______________|

			 (x4,y4)         (x3,y3) (new)
			 (m_basePt)		 (endPt)

				 (m_baseVector)
				--------------->


	Of course, we also can have following cases:

		(x4,y4)  (x3,y3)        (x2,y2)  (x1,y1)    (x3,y3)  (x4,y4)           
		 ______________			 ______________		 ______________
		|			   |		|			   |	|			   |
		|			   |		|			   |	|			   |
		|______________|		|______________|	|______________|

		(x1,y1)  (x2,y2)		(x3,y3)  (x4,y4)	(x2,y2)  (x1,y1)

*/

class ClGeometryFeature_Export RectHolePattern : public StdPatternBase, public boost::enable_shared_from_this<RectHolePattern>
{
public:
	RectHolePattern();
	RectHolePattern(const Point2D& startPt, const Point2D& endPt, double dAngle);
	RectHolePattern(LONGLONG iPatternID, const Point2D& basePt, const Vector2D& baseVector, double dOrthoRatio);
	RectHolePattern(const Point2D& basePt, const Vector2D& baseVector, double dOrthoRatio);
	virtual ~RectHolePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // implement IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_RECT_HOLE; }
	virtual BOOL IsSmooth() const override { return FALSE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;

	// notes:
	//   1) we have an assumption here! That is, start point is the left top point,
	//		end point is right bottom point. and the names of the points in this 
	//      function are based on this assumption.
	/* 
					 line1		 
				 ______________
				|			   |
		line2	|			   |  line4
				|______________|

					 line3
	*/
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public:  // implement IStdPattern interface.
	virtual Point2D GetSymmetryPt() const;
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	void SetOrthoRadio(double dOrthoRatio) { m_dOrthoRatio = dOrthoRatio; }
	double GetOrthoRadio() const { return m_dOrthoRatio; }

	// start/end point differ from the base point.
	Point2D GetStartPoint() const override;
	Point2D GetEndPoint() const override;

	Vector2D GetWidthVector() const;

	double GetLength() const { return m_baseVector.Length(); }
	void SetLength(double dVal);

	double GetWidth() const { return GetLength()*m_dOrthoRatio; }
	void SetWidth(double dVal);

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal) { m_baseVector.SetAngle(dVal); }

private:
	// the value of the (height/width).
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()
