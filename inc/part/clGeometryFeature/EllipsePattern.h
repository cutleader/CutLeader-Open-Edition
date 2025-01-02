#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(EllipsePattern)

BEGIN_CUTLEADER_NAMESPACE()

// the ellipse pattern.
// notes:
//   1) the length of the ellipse is the length "m_baseVector", and the width of the ellipse can be figured out by "m_dOrthoRatio", that is, m_dOrthoRatio = width/length.
//   2) length and width of the ellipse are determined by the draw order, please refer to the building of the ellipse in "EllipsePatternEditor".
class ClGeometryFeature_Export EllipsePattern : public StdPatternBase, public boost::enable_shared_from_this<EllipsePattern>
{
public:
	EllipsePattern(void);
	EllipsePattern(const Point2D& basePt, double dAngle, double dLength, double dWidth);
	EllipsePattern(const Point2D& basePt, const Vector2D& vect, double dOrthoRatio);
	EllipsePattern(LONGLONG iPatternID, const Point2D& basePt, const Vector2D& vect, double dOrthoRatio, int iSegNum);
	virtual ~EllipsePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_ELLIPSE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;	
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public: // get/set functions.
	void SetOrthoRadio(double dOrthoRatio) { m_dOrthoRatio = dOrthoRatio; }
	double GetOrthoRatio() const { return m_dOrthoRatio; }

	double GetLength() const;
	void SetLength(double dLength) { m_baseVector.Magnitude(dLength); }

	double GetWidth() const;
	void SetWidth(double dWidth) { m_dOrthoRatio = dWidth/GetLength(); }

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dAngle);

	int GetSegCount() const { return m_iSegNum; }
	void SetSegCount(int iVal) { m_iSegNum = iVal; }

private:
	double m_dOrthoRatio;

	// how many arcs will be used to simulate the ellipse.
	int m_iSegNum;
};

END_CUTLEADER_NAMESPACE()
