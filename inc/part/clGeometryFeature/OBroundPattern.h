#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(OBroundPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the length of the OBround is the length of "m_baseVector", and the width of the OBround can be figured out 
// by "m_dOrthoRatio", that is, m_dOrthoRatio = width/length
// notes: 
//   1) length and width of the OBround are determined by the draw order, please refer to the building of the OBround in "OBroundPatEditor".
//	 2) the "right,left,...." used in this class are based on an assumption, that is, the OBround is at zero degree.
class ClGeometryFeature_Export OBroundPattern : public StdPatternBase, public boost::enable_shared_from_this<OBroundPattern>
{
public:
	OBroundPattern(void);
	OBroundPattern(const Point2D& basePt, double dAngle, double dLength, double dWidth);
	OBroundPattern(const Point2D& basePt, const Vector2D& vect, double dOrthoRatio);
	OBroundPattern(LONGLONG iPatternID, const Point2D& basePt, const Vector2D& vect, double dOrthoRatio);
	virtual ~OBroundPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_OBROUND; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	void SetOrthoRadio(double dOrthoRatio) { m_dOrthoRatio = dOrthoRatio; }
	double GetOrthoRatio() const { return m_dOrthoRatio; }

	double GetLength() const { return m_baseVector.Length(); }
	void SetLength(double dVal);
	
	double GetWidth() const { return m_baseVector.Length()*m_dOrthoRatio; }
	void SetWidth(double dVal);

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal);

	Vector2D GetWidthVector() const;

private:
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()
