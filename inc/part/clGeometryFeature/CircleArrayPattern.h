#pragma once

#include "baseConst.h"
#include "ArrayPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(CircleArrayPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the circle array pattern.
// notes: 
//   1) the "dStartAngle" means the grid begins from this angle.

class ClGeometryFeature_Export CircleArrayPattern : public ArrayPatternBase,
											public boost::enable_shared_from_this<CircleArrayPattern>
{
public:
	CircleArrayPattern(void);
	CircleArrayPattern(const Point2D& pt, double dRadius, double dStartAngle, int iRepeatNum);
	CircleArrayPattern(PatternRelationListPtr pPatternRelations, const Point2D& pt, double dRadius, double dStartAngle, int iRepeatNum);
	CircleArrayPattern(const Point2D& pt, const Vector2D& vect, double dRadius, long iRepeatNumber);
	CircleArrayPattern(LONGLONG patID, const Point2D& pt, double dRadius, double dStartAngle, int iRepeatNum);
	virtual ~CircleArrayPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_PATTERNCIRCLE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public: // functions from IArrayPattern interface.
	virtual IPatternPtr GetFirstInstance() const;
	virtual Matrix2D GetFirstInstanceMatrix() const;
	virtual RepeatDataPtr GetRepeatData() const;

public:
	double GetRadius() const { return m_dRadius; }
	void SetRadius(double dRadius) { m_dRadius = dRadius; }

	double GetStartAngle() const { return m_baseVector.Angle(); }
	void SetStartAngle(double dAngle);

	int GetRepeatNum() const { return m_iRepeatNumber; }
	void SetRepeatNum(int iNum) { m_iRepeatNumber = iNum; }

	double GetRepeatAngle() const { return TWOPI/m_iRepeatNumber; }

private:
	void Construct(const Point2D& pt, double dRadius, double dStartAngle, int iRepeatNum);

private:
	double m_dRadius;
	long m_iRepeatNumber;
};

END_CUTLEADER_NAMESPACE()