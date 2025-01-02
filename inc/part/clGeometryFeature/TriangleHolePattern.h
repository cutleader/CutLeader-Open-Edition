#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(TriangleHolePattern)

BEGIN_CUTLEADER_NAMESPACE()

// the triangle hole pattern.
// notes: 
//   1) the length of the TriangleHolePattern is the length "m_baseVector", and 
//      the height of the TriangleHolePattern can be figured out by "m_dOrthoRatio",
//      that is, m_dOrthoRatio = height/length

/*                      
                        top point
						   8               -|-     
						  8 8               |      
						 8   8              |      
						8     8             |      
					   8       8            |      
					  8         88          | height    
					 8            8         |      
					8              8        |      
				   8                8       |      
				  8                  8      |      
				 8          8         8     |      
				888888888888888888888888   ---    
						    8  (center point) (m_basePt)
			                                       
				|      length(width)   |           
				------------------------           
				|                      |           

*/

//   2) length and height of the TriangleHolePattern are determined by the draw order,
//      please refer to the building of the TriangleHolePattern in "TRHolePatternEditor".

class ClGeometryFeature_Export TriangleHolePattern : public StdPatternBase, public boost::enable_shared_from_this<TriangleHolePattern>
{
public:
	TriangleHolePattern(void);
	TriangleHolePattern(const Point2D& centerPt, double dAngle, double dLength, double dHeight);
	TriangleHolePattern(const Point2D& centerPt, const Vector2D& vect, double dOrthoRatio);
	TriangleHolePattern(LONGLONG iPatternID, const Point2D& centerPt, const Vector2D& vect, double dOrthoRatio);
	virtual ~TriangleHolePattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_TR_HOLE; }
	virtual BOOL IsSmooth() const override { return FALSE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public:  // implement IStdPattern interface.
	virtual Point2D GetSymmetryPt() const;
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	void SetOrthoRadio(double dOrthoRatio) { m_dOrthoRatio = dOrthoRatio; }
	double GetOrthoRatio() const { return m_dOrthoRatio; }

	double GetLength() const { return m_baseVector.Length(); }
	void SetLength(double dLength) { m_baseVector.Magnitude(dLength); }

	double GetHeight() const { return GetLength()*m_dOrthoRatio; }
	void SetHeight(double dHeight) { m_dOrthoRatio = dHeight/GetLength(); }

	Vector2D GetHeightVector() const;

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dVal) { m_baseVector.SetAngle(dVal); }

private:
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()
