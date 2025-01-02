#pragma once

#include "PatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(LinePattern)
DECLARE_CUTLEADER_CLASS(IPatternVisitor)

BEGIN_CUTLEADER_NAMESPACE()


// line pattern.
class ClGeometryFeature_Export LinePattern : public PatternBase, public boost::enable_shared_from_this<LinePattern>
{
public:
	LinePattern();
	LinePattern(const Point2D& basePt, const Vector2D& vect);
	LinePattern(const Point2D& basePt, const Point2D& endPt);
	LinePattern(LONGLONG iPatternID, const Point2D& basePt, const Vector2D& vect);
	virtual ~LinePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_LINE; }
	virtual double DistanceTo(const Point2D& pt) const override;
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual Point2D GetStartPoint() const override { return m_basePt; }
	virtual void SetStartPoint(const Point2D& pt) override; // adjust the start point to "pt", not change the end point.
	virtual Point2D GetEndPoint() const override {
		Point2D pt(m_basePt.X(), m_basePt.Y());
		pt += m_baseVector;
		return pt;
	}
	virtual void SetEndPoint(const Point2D& endPt) override; // adjust the end point to "pt", not change the start point.

	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override;

	virtual void Accept(IPatternVisitorPtr patVisitor) override;

	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

public: // get/set functions.
	// get the length of the LinePattern
	double GetLength() const { return m_baseVector.Length(); }

public:
	// try to merge this pattern with "pPattern", if can not merge, return FALSE.
	// of course, "pPattern" is a line pattern.
	BOOL MergeWith(IPatternPtr pPattern, double dParallelDis);
};

END_CUTLEADER_NAMESPACE()
