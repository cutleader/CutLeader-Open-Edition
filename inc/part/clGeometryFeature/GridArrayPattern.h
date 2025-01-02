#pragma once

#include "ArrayPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(LineArrayPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the grid pattern.
// notes:
//   1) "m_baseVector" is not used for grid pattern.
class ClGeometryFeature_Export GridArrayPattern : public ArrayPatternBase, public boost::enable_shared_from_this<GridArrayPattern>
{
public:
	GridArrayPattern(void);
	GridArrayPattern(const Point2D& pt, double dRowSpace, int iRowCount, double dColumnSpace, int iColumnCount);
	GridArrayPattern(PatternRelationListPtr pPatternRelations, const Point2D& pt, double dRowSpace, int iRowCount, double dColumnSpace, int iColumnCount);
	GridArrayPattern(const Point2D& pt, double dRowSpace, long iRowCount, double dColumnSpace, long iColumnCount);
	GridArrayPattern(LONGLONG patID, const Point2D& pt, double dRowSpace, int iRowCount, double dColumnSpace, int iColumnCount);
	virtual ~GridArrayPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_PATTERNGRID; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;
	virtual void Transform(const Matrix2D& mat) override;

public: // functions from IArrayPattern interface.
	virtual IPatternPtr GetFirstInstance() const;
	virtual Matrix2D GetFirstInstanceMatrix() const;
	virtual RepeatDataPtr GetRepeatData() const; // get data column by column, when switch to other column, choose the closest one.

public: // get/set functions.
	double GetRowSpace() const { return m_dRowSpace; }
	void SetRowSpace(double d1) { m_dRowSpace = d1; }

	int GetRowCount() const { return m_iRowCount; }
	void SetRowCount(int iRowCount) { m_iRowCount = iRowCount; }

	double GetColumnSpace() const { return m_dColumnSpace; }
	void SetColumnSpace(double d1) { m_dColumnSpace = d1; }

	int GetColumnCount() const { return m_iColumnCount; }
	void SetColumnCount(int iColumnCount) { m_iColumnCount = iColumnCount; }

public:
	// merge the pattern line into the pattern grid.
	// notes: 
	//   1) we only care the pPatLine which is vertical, so in other case, do not call this.
	BOOL AddPatternLine(LineArrayPatternPtr pPatLine);

private:
	void Construct(const Point2D& pt, double dRowSpace, int iRowCount, double dColumnSpace, int iColumnCount);

private:
	double m_dRowSpace;
	int m_iRowCount;

	double m_dColumnSpace;
	int m_iColumnCount;
};

END_CUTLEADER_NAMESPACE()