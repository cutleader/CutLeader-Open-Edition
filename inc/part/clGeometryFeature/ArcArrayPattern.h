#pragma once

#include "ArrayPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

BEGIN_CUTLEADER_NAMESPACE()

// the arc array pattern.
// notes:
//   1) the start Angle will be placed in BaseVector.
class ClGeometryFeature_Export ArcArrayPattern : public ArrayPatternBase,
										 public boost::enable_shared_from_this<ArcArrayPattern>
{
public:
	ArcArrayPattern(void);
	ArcArrayPattern(const Point2D& pt, double dRadius, double dStartAngle, double dRepeatAngle, int iRepeatNum);
	ArcArrayPattern(PatternRelationListPtr pPatternRelations, const Point2D& pt, double dRadius, double dStartAngle, double dRepeatAngle, int iRepeatNum);
	ArcArrayPattern(const Point2D& pt, const Vector2D& vect, double dRadius, double dRepeatAngle, long iRepeatNumber);
	ArcArrayPattern(LONGLONG patID, const Point2D& pt, double dRadius, double dStartAngle, double dRepeatAngle, int iRepeatNum);
	virtual ~ArcArrayPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual PatternType GetPatternType() const{ return PATTERN_PATTERNARC; }
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public: // functions from IArrayPattern interface.
	virtual IPatternPtr GetFirstInstance() const;
	virtual Matrix2D GetFirstInstanceMatrix() const;
	virtual RepeatDataPtr GetRepeatData() const;

public: // set/get functions.
	double GetRadius() const { return m_dRadius; }
	void SetRadius(double dRadius) { m_dRadius = dRadius; }

	double GetStartAngle() const { return m_baseVector.Angle(); }
	void SetStartAngle(double dAngle);

	double GetMoveAngle() const { return m_baseVector.Angle() * (m_iRepeatNumber-1); }
	void SetMoveAngle(double dAngle);

	double GetRepeatAngle() const { return m_dRepeatAngle; }
	void SetRepeatAngle(double dAngle) { m_dRepeatAngle = dAngle; }

	int GetRepeatNum() const { return m_iRepeatNumber; }
	void SetRepeatNum(int iNum) { m_iRepeatNumber = iNum; }

public: // other function.
	// add a pattern to this pattern arc.
	// notes: if we found that pattern arc has been a pattern circle after adding "pPattern", return "bCircle" to caller.
	BOOL AddPattern(IPatternPtr pPattern, BOOL& bCircle);

	// get the last instance of this pattern arc.
	IPatternPtr GetLastInst() const;

private:
	void Construct(const Point2D& pt, double dRadius, double dStartAngle, double dRepeatAngle, int iRepeatNum);

private:
	double m_dRadius;
	double m_dRepeatAngle;
	int m_iRepeatNumber;
};

END_CUTLEADER_NAMESPACE()
