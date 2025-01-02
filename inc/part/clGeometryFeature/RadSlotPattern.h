#pragma once

#include "StdPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(RadSlotPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the radius slot pattern.
// notes: 
//   1) the radius slot pattern is something similar with the arc pattern. the "m_dWidthRatio" variable is added to figure out the width of the radius 
//      slot, the other variables are same with arc pattern, please refer to the arc pattern for detail.
/*
                                  
				   888888888                  
				  8         88888             
				 8               88           
				  8                88         
				   888888888         88       
							   88       8     
								  88     8    
								   8     8    
					  |   radius   8     8   
					 ------------------   |   
					  |             |    8|   
								   8|    8|   
							   88   |  8  |   
							888     |88   |   
				   888888888       8|     |   
				 8               88 |     |   
				 8          88888   |     |   
				   888888888        |     |   
									|     |   
									|     |   width

*/
class ClGeometryFeature_Export RadSlotPattern : public StdPatternBase, public boost::enable_shared_from_this<RadSlotPattern>
{
public:
	RadSlotPattern(void);
	RadSlotPattern(const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle, double dWidth);
	RadSlotPattern(LONGLONG iPatternID, const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle, double dWidth);
	RadSlotPattern(const Point2D& basePt, const Vector2D& baseVector, const Vector2D& midVector, double dWidthRatio);
	virtual ~RadSlotPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_RADSLOT; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsSameShape(IPatternPtr pPattern) const override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;	
	virtual void Accept(IPatternVisitorPtr patVisitor) override;
	virtual void Transform(const Matrix2D& mat) override;

public:  // implement IStdPattern interface.
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal);

public:
	double GetRadius() const { return m_baseVector.Length(); }
	void SetRadius(double dRadius);

	double GetStartAngle() const { return m_baseVector.Angle(); }
	void SetStartAngle(double dAngle);

	double GetMoveAngle() const;
	void SetMoveAngle(double dAngle);

	double GetWidthRatio() const { return m_dWidthRatio; }

	double GetWidth() const { return GetRadius() * m_dWidthRatio; }
	void SetWidth(double dWidth);

	//get the middle vector.
	Vector2D GetMidVector() const { return m_MidVector; }

private:
	void Construct(const Point2D& basePt, double dRadius, double dStartAngle, double dMoveAngle, double dWidth);

private:
	// the middle vector of the arc.
	Vector2D m_MidVector;

	double m_dWidthRatio;
};

END_CUTLEADER_NAMESPACE()
