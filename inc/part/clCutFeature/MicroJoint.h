#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include "SubjectBase.h"
#include "Matrix2D.h"
#include "PatternPosition.h"


DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(MicroJoint)


BEGIN_CUTLEADER_NAMESPACE()


// “微连接特征”。
// 注:
//    1) we do not have micro joint data for open path.
//    2) 父亲ID中记录了“回路微连接特征”ID。
//    3) 微连接的刺穿点条件用当前轮廓的刺穿点条件。
class ClCutFeature_Export MicroJoint : public ClData, public SubjectBase
{
public:
	MicroJoint(void);
	MicroJoint(LONGLONG iPatId, const PatternPosition& patternPosition, double dWidth);
	MicroJoint(const MicroJoint& microJoint); // 不克隆条件。
	virtual ~MicroJoint(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;

	// clone the micro joint.
	// notes:
	//   1) we also need to clone the condition nodes on the lead.
	//   2) 保持和原有微连接特征同样的父亲ID。
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetPatternID(LONGLONG iPatId) { m_iPatternID = iPatId; }
	LONGLONG GetPatternID() const { return m_iPatternID; }

	PatternPosition GetPatternPosition() const { return m_patternPosition; }
	void SetPatternPosition(const PatternPosition& patternPosition) { m_patternPosition = patternPosition; }

	double GetWidth() const { return m_dWidth; }
	void SetWidth(double dWidth) { m_dWidth = dWidth; }

	void EnableLeadIn(BOOL b) { m_bLeadIn = b; }
	BOOL EnableLeadIn() const { return m_bLeadIn; }

	void SetLeadInLen(double dLength) { m_leadInLen = dLength; }
	double GetLeadInLen() const { return m_leadInLen; }

	void SetLeadInAngle(double dAngle) { m_leadInAngle = dAngle; }
	double GetLeadInAngle() const { return m_leadInAngle; }

	void EnableLeadOut(BOOL b) { m_bLeadOut = b; }
	BOOL EnableLeadOut() const { return m_bLeadOut; }

	void SetLeadOutLen(double dLength) { m_leadOutLen = dLength; }
	double GetLeadOutLen() const { return m_leadOutLen; }

	void SetLeadOutAngle(double dAngle) { m_leadOutAngle = dAngle; }
	double GetLeadOutAngle() const { return m_leadOutAngle; }

public: // other functions.
	/************************************************************************/
	// about getting some points.

	// get two "micro joint lead point" of the micro joint, you know, we should deduct the width/2.
	/* 
					  *        *           
					  *        *           
		   ********************************
				  pt1 *        * pt2          
					  *        *       Point "pt1" and "pt2" are what we need.    
	*/
	// notes: 
	//   1) the first item is the lead out point, and the second item is lead in point.
	//   2) if micro joint is on the arc, we need to adjust "pt1" and "pt2".
	/* params:
	*   1) pHostGeom: the host geom, we need it for arc.
	*   2) jointPt : the joint point on the loop.
	*   3) vect : the cut direction on the joint point.
	*/
	Point2DListPtr GetMicroJointLeadPt(const ILineArc2D* pHostGeom, const Point2D& jointPt, const Vector2D& vect) const;

	// get micro joint point.
	/*              
                  *        *                
                  *   pt   *      Point "pt" is what we need.
                  *   *    *                
       ***********************************  
                  *   *    *                
                  *        *                
	*/
	// notes: 
	//   1) under sheet mode, we need "mat".
	//   2) for grid pattern, we should use the first instance.
	Point2D GetMicroJointPt(const IPattern* pPattern, const Matrix2D& mat = Matrix2D()) const;
	/************************************************************************/


	// whether this micro joint is closer to "pt" than "pMicroJoint".
	// notes: under sheet mode, we need "mat".
	/* param: 
	*  pPattern and mat: use them to get micro joint point.
	*/
	BOOL Closer(const MicroJoint* pMicroJoint, const Point2D& pt, const IPattern* pPattern, const Matrix2D& mat = Matrix2D());

	// get the line/arcs of the micro joint.
	/*
			 *							 *
			 *							/*\  cut direction
			 *  to						 *
		************	toLine  		 *      
		************	fromLine    	 *
			 *  from					 *
			 *         
			 *         
	*/
	// notes: 
	//   1) the line/arcs returned follow the cut direction.
	/* params:
	*   1) pHostGeom: the host geom, we need it for arc.
	*   2) startPt : the joint point on the loop.
	*   3) vect : the cut direction on the joint point.
	*   4) emPatternLoopCutSide: the cut side of the loop, if this is "PatternLoopCutSide_On", "m_bLeadIn" and "m_bLeadOut" will already be FALSE.
	*   5) bForView: if FALSE, we do not create the two lines which are vertical to the cut direction. 
	*/
	LineArc2DListPtr GetLineArcs(const ILineArc2D* pHostGeom, const Point2D& jointPt, const Vector2D& vect, PatternLoopCutSideType emPatternLoopCutSide, double dSimuSize, BOOL bForView = TRUE) const;

	// the distance from this micro joint to "pt".
	// notes: under sheet mode, we need "mat".
	/* param: 
	*  pPattern and mat: use them to get micro joint point.
	*/
	double DistanceTo(const Point2D& pt, IPatternPtr pPattern, const Matrix2D& mat = Matrix2D());

	// 得到这个微连接点在圆弧“pPattern”上的位置百分比。
	double ArcPercent(Arc2DPtr pArc2D, IPatternPtr pPattern, const Matrix2D& mat = Matrix2D());

private:
	/************************************************************************/
	// the micro joint position
	// notes: 
	//   1) for grid pattern, "m_iPatternID" is the ID of the grid pattern,
	//      "m_patternPosition" is the position at the first instance of the grid pattern.

	// the pattern which contains the micro joint.
	LONGLONG m_iPatternID;

	// the position of the micro joint.
	PatternPosition m_patternPosition;
	/************************************************************************/


	/************************************************************************/
	// lead in/out info for micro joint.
	// notes: 
	//   1) angles here are degree, not radius.
	//   2) you know, in the scheme of current part/sheet, we also keep such data.

	BOOL m_bLeadIn;
	double m_leadInLen;
	double m_leadInAngle;

	BOOL m_bLeadOut;
	double m_leadOutLen;
	double m_leadOutAngle;
	/************************************************************************/


    // the width of the micro joint.
    double m_dWidth;
};

END_CUTLEADER_NAMESPACE()
