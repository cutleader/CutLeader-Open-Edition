#pragma once

#include "IPattern.h"
#include "PatternRelationList.h"
#include "PatternRelation.h"
#include "SubjectBase.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export PatternBase : public ClData, virtual public IPattern, public SubjectBase
{
public:
	PatternBase();
	virtual ~PatternBase(void);

public: // implement IPattern interface.
	virtual Point2D GetBasePoint() const { return m_basePt; }
	virtual void SetBasePoint(const Point2D& pt) override { m_basePt = pt; }
	virtual Vector2D GetBaseVector() const override { return m_baseVector; }
	virtual void SetBaseVector(const Vector2D& vect) override { m_baseVector = vect; }
	virtual BOOL IsStdPattern() const override { return FALSE; }
	virtual BOOL IsArrayPattern() const { return FALSE; }
	virtual BOOL IsSmooth() const override { return TRUE; }

	virtual Point2D GetSnapPtClosestTo(const Point2D& pt) const override;
	virtual Point2DPtr GetSnapPtClosestTo(const Point2D& pt, double dCaptureRange) const override;
	virtual Point2D GetPointByPatPos(const PatternPosition& patternPosition) const override;
	virtual PatternPosition GetPatPosByPoint(const Point2D& pt, PatternPosition& secondPatPos) const override;
	virtual Point2D GetClosestPt(const Point2D& pt, const Matrix2D& mat = Matrix2D()) const override;
	virtual Point2D GetStartPoint() const override { return Point2D(); }
	virtual void SetStartPoint(const Point2D& pt) override {}
	virtual Point2D GetEndPoint() const override { return Point2D(); }
	virtual void SetEndPoint(const Point2D& pt) override {}
	virtual double DistanceTo(const Point2D& pt) const override;

	virtual PatternRelationListPtr GetPatternRelationList() const override { return m_pPatternRelations; }
	virtual void AddPatternRelation(PatternRelationPtr pPatternRelation) override { m_pPatternRelations->AddPatternRelation(pPatternRelation); }

	virtual bool IsControlPat() const override;
	virtual void SetBeControlPat(IPatternPtr pPattern) override;

	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override { return FALSE; }
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override { return FALSE; }

	virtual void Transform(const Matrix2D& mat) override;

	virtual CString GetLayerName() const override { return m_strLayerName; }
	virtual void SetLayerName(CString strLayerName) override { m_strLayerName = strLayerName; }
	virtual int GetColorIndex() const override { return m_iColorIndex; }
	virtual void SetColorIndex(int iColorIndex) override { m_iColorIndex = iColorIndex; }

protected:
	// use the relation of this pattern to update "pPattern".
	void UpdateRelation(IPatternPtr pPattern) const;

protected:
	// the base point for all type pattern.
	Point2D m_basePt;

	// the base vector for all type pattern.
	Vector2D m_baseVector;

	// keep the relations of this pattern
	PatternRelationListPtr m_pPatternRelations;

	// 从dxf导入图形时，记录图层和颜色信息，这些信息只在导入后做工艺分配时使用。
	CString m_strLayerName;
	int m_iColorIndex;
};

END_CUTLEADER_NAMESPACE()
