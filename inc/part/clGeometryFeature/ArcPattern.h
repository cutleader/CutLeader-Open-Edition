#pragma once

#include "PatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(ArcPattern)

BEGIN_CUTLEADER_NAMESPACE()

// arc pattern.
class ClGeometryFeature_Export ArcPattern : public PatternBase,
									  public boost::enable_shared_from_this<ArcPattern>
{
public:
	ArcPattern(void);
	ArcPattern(const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle);
	ArcPattern(LONGLONG iPatternID, const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle);
	ArcPattern(const Point2D& basePt, const Vector2D& baseVector, const Vector2D& midVector, BOOL bCircle, DIRECTION circleDir);
	virtual ~ArcPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_ARC; }
	virtual double DistanceTo(const Point2D& pt) const override;
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual Point2D GetStartPoint() const override;
	virtual Point2D GetEndPoint() const override;
	virtual void SetStartPoint(const Point2D& pt) override; // adjust the start point to "pt", not change the sweep angle and end point. cannot be used for circle.
	virtual void SetEndPoint(const Point2D& pt) override; // adjust the end point to "pt", not change the sweep angle and start point. cannot be used for circle.

	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override;

	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

	virtual void Accept(IPatternVisitorPtr patVisitor) override;
	virtual void Transform(const Matrix2D& mat) override;

public: // get/set functions
	double GetRadius() const {  return m_baseVector.Length(); }
	void SetRadius(double dVal) { m_baseVector.Magnitude(dVal); }

	double GetStartAngle() const { return m_baseVector.Angle(); }
	void SetStartAngle(double dVal);

	double GetMoveAngle() const;
	void SetMoveAngle(double dVal);

	double GetEndAngle() const { return (GetStartAngle()+GetMoveAngle()); }

	Point2D GetMiddlePoint() const;

	// get the middle vector.
	Vector2D GetMidVector() const { return m_MidVector; }

public:
	// try to merge this pattern with "pPattern", if can not merge, return FALSE.
	// of course, "pPattern" is an arc pattern.
	BOOL MergeWith(IPatternPtr pPattern);

private:
	void Construct(const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle);

private:
	// the middle vector of the arc.
	Vector2D m_MidVector;

	/************************************************************************/
	// about circle.

	// whether is circle.
	BOOL m_bCircle;

	// the direction of the circle.
	DIRECTION m_circleDir;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
