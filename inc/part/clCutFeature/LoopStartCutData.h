#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "ClData.h"
#include "SubjectBase.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(LoopStartCutCache)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// ��·����������
// ע: 
//    1) �������а���������Ϣ:
//       a) ��·�������������á�
//       b) ���е�λ�ú��и��
//       c) �����С�
//    2) �����е��ڽ���ʱ:
//       a) lead type cannot be changed, they are hard coded.
//       b) over cut for lead out cannot be used.
//       c) about the line/arc of the lead at corner, refer to the illustration, for case1 we can extend the geom at corner to form the lead, for case2, we need to use
//          the angle bisector to form the lead.
/*    
									  **                       
									 *  *                      
									*    *                     
								   *      **                   
								  *         *                  
								 *   *       *                 
								*   **        *                
							   *    * *        *               
							  *    *  *         **             
							 *     *  *     **    *            
							*     *    *  ** *     *           
						   *      *     **   *      *          
						  *      *   start  *        *         
						 *       *          *         *        
						*       *           *          **      
					   *        *                        *     
					  *        **************             *    
					  **************************************   start
								  
								   case 1

					  		           **                  
									  *  *                                             
									 *     *                        
									*       *              
								   *   start *             
								  *   **      *            
								  *  *  **    *            
								 *   *    *    *           
								 *  *      *    *          
								*   *       **  *          
								*  *          *  *         
							   *    ************  *        
							   *                   *           
							 *          **          *      
							 *       **    **        *     
							*      **  start **      *     
							*    **            ***    *    
						   *   **                 ***  *   
						   * **                      * *   
							*                          *   
							       case 2

*/
class ClCutFeature_Export LoopStartCutData : public ClData, public SubjectBase
{
public:
	LoopStartCutData(IPatternLoopPtr pPatternLoop);
	virtual ~LoopStartCutData(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;

	// clone start/end cut info of the loop.
	// notes:
	//   1) this function do not clone pattern loop.
	virtual IDataPtr Clone() const override;

public: // set/get functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

	void SetLoopStartCutPt(ILoopStartCutPtPtr pLoopStartCutPt) { m_pLoopStartCutPt = pLoopStartCutPt; }
	ILoopStartCutPtPtr GetLoopStartCutPt() const { return m_pLoopStartCutPt; }

	PatternLoopCutSideType GetLoopCutSide() const { return m_emPatternLoopCutSide; }
	void SetLoopCutSide(PatternLoopCutSideType emPatternLoopCutSide) { m_emPatternLoopCutSide = emPatternLoopCutSide; }

	ILeadPtr GetLeadIn() const { return m_pLeadIn; }
	void SetLeadIn(ILeadPtr leadIn) { m_pLeadIn = leadIn; }

	ILeadPtr GetLeadOut() const { return m_pLeadOut; }
	void SetLeadOut(ILeadPtr leadOut) { m_pLeadOut = leadOut; }

	void SetCacheData(LoopStartCutCachePtr pStaEndCutCache) { m_pLoopStartCutCache = pStaEndCutCache; }
	LoopStartCutCachePtr GetCacheData() const { return m_pLoopStartCutCache; }

public:
	// get the end cut point.
	// notes:
	//   1) this function will use the start cut point in start/end cut data cache.
	//   2) we should care the over cut.
	//   3) if array loop and bArrayLastInst is TRUE, just for last inst.
	Point2D CalcEndCutPt(BOOL bArrayLastInst = FALSE) const;

	// get the lead in or lead out.
	ILeadPtr GetLeadByType(LeadInOutType emLeadInOut) const;

	// whether the cut direction is same with the loop direction.
	// notes:
	//   1) this function will use the start cut point in start/end cut data cache.
	BOOL SameDirWithLoop() const;

    // ��������������ǲ��Ǽ��ڽ��ϣ����ҹ���Ϊ��ֵ��
    bool IsLeadAtCorner_withNegtiveOverCut() const;

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

	// ��·���иʼ�ͽ����㡣
	ILoopStartCutPtPtr m_pLoopStartCutPt;

	// the cut side of the loop.
	// notes: 
	//   1) for open path, always be "PatternLoopCutSide_On".
	PatternLoopCutSideType m_emPatternLoopCutSide;

	// lead in/out of the loop.
	ILeadPtr m_pLeadIn;
	ILeadPtr m_pLeadOut;

	// the cache data.
	LoopStartCutCachePtr m_pLoopStartCutCache;
};

END_CUTLEADER_NAMESPACE()
