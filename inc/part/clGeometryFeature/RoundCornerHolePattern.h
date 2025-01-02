#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(RoundCornerHolePattern)

BEGIN_CUTLEADER_NAMESPACE()

// the round corner hole pattern.
// notes:
//    1) BasePoint hold the center point.
//    2) BaseVector hold the Length/2(H-direction) and the angle.
class ClGeometryFeature_Export RoundCornerHolePattern : public StdPatternBase, public boost::enable_shared_from_this<RoundCornerHolePattern>
{
public:
	RoundCornerHolePattern();
	RoundCornerHolePattern(const Point2D& centerPt, double dLength, double dWidth, double dRadius, double dAngle);
	RoundCornerHolePattern(LONGLONG iPatternID, const Point2D& centerPt, double dLength, double dWidth, double dRadius, double dAngle);
	virtual ~RoundCornerHolePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_RCHOLE; }

	/*						  4	
						  5
						1	  0		3	if angle = 0

							  2	
	*/
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

	// the illustration based on an assume that the angle is zero.
	/*
	    corner2      line1        corner1
		  /-----------------------\ 
		  |                       | 
		  |                       | 
		  |                       | 
		  | line2                 | line4
		  |                       | 
		  |                       | 
		  |        line3          | 
		  \-----------------------/ 
		corner3                  corner4
	*/
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	Vector2D GetWidthVector() const;

	double GetLength() const { return m_baseVector.Length()*2; }
	void SetLength(double dVal) { m_baseVector.Magnitude(dVal/2); }

	double GetWidth() const { return m_baseVector.Length()*2*m_dOrthoRatio; }
	void SetWidth(double dVal);

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal) { m_baseVector.SetAngle(dVal); }

	double GetOrthoRatio() const { return m_dOrthoRatio; }
	double GetArcRatio() const { return m_dArcRatio; }

	double GetArcRadius() const;
	void SetArcRadius(double dVal);

private:
	// the ortho ratio is Width/Length.
	double m_dOrthoRatio;

	// the arc ratio is radius/(Length/2).
	double m_dArcRatio;
};

END_CUTLEADER_NAMESPACE()
