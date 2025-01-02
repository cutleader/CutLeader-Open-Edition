#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopMicroJointList : public std::vector<LoopMicroJointPtr>,
									  public ClData
{
public:
	LoopMicroJointList(void);
	~LoopMicroJointList(void);

public: // implement IData interface.
	// clone the LoopMicroJointList.
	IDataPtr Clone() const override;

public:
	/************************************************************************/
	// 关于得到“回路微连接特征”。

	// 根据回路ID得到“回路微连接特征”对象。
	LoopMicroJointPtr GetLoopMicroJointByLoop(LONGLONG iLoopId);

	// 根据“回路微连接特征”ID得到“回路微连接特征”对象。
	LoopMicroJointPtr GetItemByID(LONGLONG iLoopMicroJointId);

	// 根据微连接点得到“回路微连接特征”对象。
	LoopMicroJointPtr GetLoopMicroJointByMJPt(Point2D bmjPt);
	/************************************************************************/

	// 得到所有微连接点。
	Point2DListPtr GetAllMicroJointPts();

	// 得到所有“微连接点”对应的“微连接特征”。
	std::vector<std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs();

	// 根据微连接点得到回路对象。
	IPatternLoopPtr GetLoopByMicroJointPt(Point2D bmjPt);
};

END_CUTLEADER_NAMESPACE()
