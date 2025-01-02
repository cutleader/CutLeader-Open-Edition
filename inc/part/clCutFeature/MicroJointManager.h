#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)
DECLARE_CUTLEADER_CLASS(MicroJointRange)
DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// “微连接特征”管理器。
class ClCutFeature_Export MicroJointManager
{
public:
	// 创建“微连接特征”。
	// 注: 
	//   1) 调用者只要提供微连接点位置和微连接宽度，其他信息来自参数配置。
	static MicroJointPtr BuildMicroJoint(LoopMicroJointPtr pLoopMicroJoint, int iPatIndex, const Point2D& jointPos, double dSize,
		const MicroJointConfigItem* pMicroJointConfigItem);


	/************************************************************************/
	// 有关微连接宽度。

	// 检查图形上面能不能放得下指定宽度的“微连接特征”。
	// 注:
	//   1) the line which can hold the micro joint should twice as long as "dMicroJointWidth", and the left/right side of the joint pt should longer than "dMicroJointWidth/2".
	//   2) the arc which can hold the micro joint should fit:
	//      a) arc length > dMicroJointWidth*2.
	//      b) arc diameter > dMicroJointWidth*2.
	//      c) "tmpFromLine/tmpToLine" line should intersect will arc, refer to "MicroJoint::GetLineArcs()".
	static BOOL GeomCanHoldMJ(IPatternPtr pPattern, Point2D jointPt, double dMicroJointWidth);

	// 是否可以修改“微连接特征”的宽度。
	static BOOL CanChangeMJWidth(IPatternLoopPtr pPatternLoop, MicroJointPtr pMJ, double dNewWidth);

	// 是否可以修改“微连接特征”的宽度。
	// 注：
	//   1) 只要有一个“微连接特征”不能调整宽度就返回FALSE.
	static BOOL CanChangeMJWidth(LoopMicroJointListPtr pLoopMicroJointList, MicroJointListPtr pMicroJointList, double dNewWidth);
	/************************************************************************/


	// 给回路分配“微连接特征”。
	static void AssignLoopMicroJoint(LoopMicroJointPtr pLoopMicroJoint, const MicroJointConfigItem* pMicroJointConfigItem,
		const LoopTopologyItemList* pLoopTopologyItems);

	// 删除无效的“微连接特征”。
	// 注：
	//   1) 如果有无效的“微连接特征”就返回TRUE。
	static BOOL DelInvalidMJ(LoopMicroJointPtr pLoopMicroJoint);

private:
	// assign the loop micro joint according to one range.
	/* params:
	 * 1) pMicroJointRange : one range of 3.
	 * 2) bSkip : if this range contains the loop, and the Joint point quantity is zero, we should skip this loop.
	*/
	// notes: 
	//   1) we know we have 3 ranges defined, but some ranges do not contain the loop.
	//   2) if loop is assigned successfully, return TRUE, otherwise return FALSE.
	static BOOL AssignMicroJointInfo(LoopMicroJointPtr pLoopMicroJoint, const MicroJointConfigItem* pMicroJointConfigItem,
		const MicroJointRange* pMicroJointRange, const LoopTopologyItemList* pLoopTopologyItems, BOOL& bSkip);
};

END_CUTLEADER_NAMESPACE()
