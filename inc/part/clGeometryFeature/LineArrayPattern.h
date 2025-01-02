#pragma once

#include "ArrayPatternBase.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(LineArrayPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the line grid.
/*   
                                                  ****         
                                                **    **       
                                               *        *      
                                              *          *     
                             ****             *          *     
                           **    **            *        *      
                          *        *            **    **       
                         *          *             ****         
        ****             *          *                          
      **    **            *        *                           
     * basePt *            **    **                            
    *   _|_    *             ****                              
    *    |     *                                               
     *        *                                                
      **    **                                                 
        ****                                                   
                             **                                
                       ******                                  
                  *****                                        
            ******                                             
         ***                                                   
         this vector keep the space and angle.

*/
// notes:
//   1) the space and angle will be placed in "m_baseVector".
//   2) we use the base point of the control pattern as the base point,
//      of course we can move the PatternLine to change the base point,
//      in the illustration, the base point is always the base point of the
//      control pattern(Circle).

class ClGeometryFeature_Export LineArrayPattern : public ArrayPatternBase, public boost::enable_shared_from_this<LineArrayPattern>
{
public:
	LineArrayPattern(void);
	LineArrayPattern(Point2D pt, double dSpace, double dAngle, int iRepeatNum);
	LineArrayPattern(PatternRelationListPtr pPatternRelations, Point2D pt, double dSpace, double dAngle, int iRepeatNum);
	LineArrayPattern(LONGLONG patID, Point2D pt, double dSpace, double dAngle, int iRepeatNum);
	LineArrayPattern(Point2D pt, Vector2D vect, int iRepeatNum);
	virtual ~LineArrayPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public: // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_PATTERNLINE; }
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual LineArc2DListPtr Calculate2DLineArcs() const override;;
	virtual void Accept(IPatternVisitorPtr patVisitor) override;

public: // functions from IArrayPattern interface.
	virtual IPatternPtr GetFirstInstance() const;
	virtual Matrix2D GetFirstInstanceMatrix() const;
	virtual RepeatDataPtr GetRepeatData() const;

public: // set/get functions.
	double GetXSpace() const { return m_baseVector.X(); }
	void SetXSpace(double dSpace) { m_baseVector.X(dSpace); }

	double GetYSpace() const { return m_baseVector.Y(); }
	void SetYSpace(double dSpace) { m_baseVector.Y(dSpace); }

	double GetSpace() const { return m_baseVector.Length(); }
	void SetSpace(double dSpace);

	double GetAngle() const { return m_baseVector.Angle(); }
	void SetAngle(double dAngle);

	int GetRepeatNum() const { return m_iRepeatNumber; }
	void SetRepeatNum(int iNum) { m_iRepeatNumber = iNum; }

public: // other function.
	// add a pattern to this pattern line.
	BOOL AddPattern(IPatternPtr pPattern);

private:
	// in the illustration, repeat num is 3.
	int m_iRepeatNumber;
};

END_CUTLEADER_NAMESPACE()
