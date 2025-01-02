#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "PatternPosition.h"
#include "ClData.h"
#include "SubjectBase.h"

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

// “回路微连接特征”信息。
class ClCutFeature_Export LoopMicroJoint : public ClData, public SubjectBase
{
public:
	LoopMicroJoint(IPatternLoopPtr pPatternLoop);
	~LoopMicroJoint(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the micro joint on the loop.
	// notes:
	//   1) do not clone pattern loop.
	//   2) 需要更新每个新的特征微连接特征的父亲ID。
	IDataPtr Clone() const override;

public: // set/get functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop; }
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }

	void SetMicroJointList(MicroJointListPtr pMicroJointList) { m_pMicroJointList = pMicroJointList; }
	MicroJointListPtr GetMicroJointList() const { return m_pMicroJointList; }

public: // other functions.
	/************************************************************************/
	// “微连接特征”的二维直线和圆弧表达。
	// notes: 
	//   1) bForView: if FALSE, we do not create the two lines which are vertical to the cut direction. 

	// get Line/Arcs of all MicroJoints on the loop. 
	LineArc2DListPtr GetMicroJointLineArcs(BOOL bFollowLoopDirection, PatternLoopCutSideType emPatternLoopCutSide, double dSimuSize, BOOL bForView = TRUE) const;

	// get Line/Arcs of the specified micro joint.
	// notes: 
	//   1) you know, we only support line type lead, so if this micro joint support lead in/out, this function will return two items.
	LineArc2DListPtr GetMicroJointLineArcs(const MicroJoint* pMicroJoint, BOOL bFollowLoopDirection, PatternLoopCutSideType emPatternLoopCutSide, double dSimuSize, BOOL bForView = TRUE) const;
	/************************************************************************/


	// 得到所有的“微连接点”。
	Point2DListPtr GetAllMicroJointPts() const;

	// 得到所有“微连接点”对应的“微连接特征”。
	std::vector<std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs() const;

	// get two "micro joint lead point" of the specified micro joint, you know, we should deduct the width/2.
	// notes: 
	//   1) the first item is the lead out point, and the second item is lead in point.
	Point2DListPtr GetMicroJointLeadPt(const MicroJoint* pMicroJoint, BOOL bFollowLoopDirection) const;

	// 得到微连接点对应的“微连接特征”。
	MicroJointPtr GetMicroJointAtLocation(const Point2D& pt) const;

private:
	// the relative pattern loop.
	IPatternLoopPtr m_pPatternLoop;

	// the micro joint list
	MicroJointListPtr m_pMicroJointList;
};

END_CUTLEADER_NAMESPACE()
