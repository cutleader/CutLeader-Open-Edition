#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(DoubleDPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the double-D class.
// notes: 
//   1) the variables in this class are similar with those in SingleD class.
//   2) the "width" is double size of the distance from center point to chord.
//   3) the "angle" is the angle of chord.
class ClGeometryFeature_Export DoubleDPattern : public StdPatternBase,
										  public boost::enable_shared_from_this<DoubleDPattern>
{
public:
	DoubleDPattern(void);
	DoubleDPattern(const Point2D& centerPt, double dRadius, double dChord, double dAngle);
	DoubleDPattern(LONGLONG patID, Point2D& centerPt, double dRadius, double dChord, double dAngle);
	~DoubleDPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_DOUBLED; }
	virtual BOOL IsSmooth() const override { return FALSE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal);

	double GetRadius() const { return m_baseVector.Length(); }
	void SetRadius(double dVal) { m_baseVector.Magnitude(dVal); }

	// Ô²ÐÄµ½ÏÒµÄ¾àÀë¡£
	double GetToChord() const { return GetRadius()*m_dOrthoRatio; }

	double GetWidth() const { return GetToChord()*2; }
	void SetWidth(double dVal);

	void SetOrthoRadio(double dOrthoRatio) { m_dOrthoRatio = dOrthoRatio; }
	double GetOrthoRatio() const { return m_dOrthoRatio; }

	Vector2D GetToChordVector() const;

private:
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()
