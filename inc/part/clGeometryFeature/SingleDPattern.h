#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(SingleDPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the single-D class.
/*
             ____
			/    \
		   |      |
		    \    /
		     \__/
*/
// notes:
//   1) BasePoint hold the center point, BaseVector hold the radius and the angle.
//      OrthoRatio is dChord<distance from the chord to center>/radius.
//   2) we define that the "top point" is the middle point of the chord.
//      Refer to the illustration, so we can get left, bottom, right point.

class ClGeometryFeature_Export SingleDPattern : public StdPatternBase, public boost::enable_shared_from_this<SingleDPattern>
{
public:
	SingleDPattern(void);
	SingleDPattern(const Point2D& centerPt, double dRadius, double dChord, double dAngle);
	SingleDPattern(LONGLONG patID, const Point2D& centerPt, double dRadius, double dChord, double dAngle);
	virtual ~SingleDPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_SINGLED; }
	virtual BOOL IsSmooth() const override { return FALSE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public: // set/get functions.
	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal);

	double GetRadius() const { return m_baseVector.Length(); }
	void SetRadius(double dVal) { m_baseVector.Magnitude(dVal); }

	// Ô²ÐÄµ½ÏÒµÄ¾àÀë¡£
	double GetToChord() const { return GetRadius()*m_dOrthoRatio; }
	
	double GetWidth() const { return GetRadius()+GetToChord(); }
	void SetWidth(double dVal);

	double GetOrthoRatio() const { return m_dOrthoRatio; }

	Vector2D GetToChordVector() const;

private:
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()
