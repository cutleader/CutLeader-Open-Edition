#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(CirclePattern)

BEGIN_CUTLEADER_NAMESPACE()

// the round hole pattern.
// notes:
//   1) now "GetLineArcs()" return an Arc2D(circle), the start angle of this arc is 0, in sequence module, when generate the cut node for CirclePattern
//      instance, we should adjust the start angle to the start cut point.
class ClGeometryFeature_Export CirclePattern : public StdPatternBase,
										 public boost::enable_shared_from_this<CirclePattern>
{
public:
	CirclePattern(void);
	CirclePattern(const Point2D& centerPt, double dRadius);
	CirclePattern(LONGLONG patId, const Point2D& centerPt, double dRadius);
	virtual ~CirclePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // implement IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_ROUNDHOLE; }
	virtual double DistanceTo(const Point2D& pt) const override;
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public: // set/get functions.
	double GetRadius() const;
	void SetRadius(double dRadius) { m_baseVector.Magnitude(dRadius); }
};

END_CUTLEADER_NAMESPACE()
