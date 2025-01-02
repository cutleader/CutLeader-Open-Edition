#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(MachineParam)

BEGIN_CUTLEADER_NAMESPACE()

// 避让路径管理器。
class ClCutSequence_Export TraceDataMgr
{
public:
    // 计算从"lastCutPt"到工序开始点的避让路径(快速移动点)。
    static void ProcessAvoidPath(const Point2D& lastCutPt, ICutSequencePtr pCutSequence, const LoopInstanceList* pSeqLoopInsts, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

    // 按顺序生成这些工序之间的避让路径
    // 注：
    //  1) pNewCutSeqList是自动工序后新生成的工序，而且此时可能做过了“切割点自动调整”，但是还没做过避让路径。这个集合中必须有东西。
    //  2) pOldCutSeqs是pNewCutSeqList之前的那些工序，当然集合中可能没有元素。
    static void ProcessAvoidPath(const CutSequenceList* pOldCutSeqs, CutSequenceListPtr pNewCutSeqList, const ConfigData* pConfigData);

	// 构造避让数据。
	static AvoidDataPtr BuildAvoidData(const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem, const MachineParam* pMachineParam);


	/************************************************************************/
	// 关于计算快速移动点。

	// calculate the avoid points from "startPt" to "endPt".
	// notes:
	//   1) if no extra trace line is needed between "startPt" and "endPt", 0 item will be returned.
	//   2) if fail to avoid from "startPt" to "endPt", 0 item will be returned.
	static Point2DListPtr CalcAvoidPtList(const Point2D& startPt, const Point2D& endPt, const LoopInstanceList* pLoopInstList,
		AvoidDataPtr pAvoidData, BOOL& bFailAvoid);

	// calculate the avoid points from "startPt" to "endPt".
	/* params:
	*  pGeomLoopList: the loops should be avoided.
	*  rangeRect: 运动点被限制的范围。
	*/
	// notes:
	//   1) if no extra trace line is needed between "startPt" and "endPt", 0 item will be returned.
	//   2) if fail to avoid from "startPt" to "endPt", 0 item will be returned.
	static Point2DListPtr CalcAvoidPtList(const Point2D& startPt, const Point2D& endPt, LineArc2DLoopListPtr pGeomLoopList,
		const Rect2D& rangeRect, double dAvoidDis, BOOL& bFailAvoid);
	/************************************************************************/


	// 计算从startPt到endPt的移动点所允许的范围。
	static Rect2D GetAvoidPtRange(const Point2D& startPt, const Point2D& endPt, AvoidDataPtr pAvoidData);

	// get the loops which will be avoided.
	/* params:
	*  pSeqLoopInsts: the loop insts which have been sequenced, we should avoid them.
	*  rangeRect: the max rect range which contains the trace line path.
	*/
	// notes:
	//   1) we should filter "pSeqLoopInsts" using "rangeRect", you know, we only should avoid the loops which intersect with "rangeRect".
    //   2) 这个函数会跳过开放轮廓，不需要避开开放轮廓。
	static LineArc2DLoopListPtr GetAvoidLoopList(const LoopInstanceList* pSeqLoopInsts, const Rect2D& rangeRect);

	// 得到所有工序之间的快速移动点。
	// 注：
	//  1) 对应某个工序保存这些信息：<上个工序的结束点, 中间的点..., 这个工序的开始点>。
	static std::map<LONGLONG, Point2DListPtr> GetTracePtData(const Point2D& startCutPt, CutSequenceListPtr pCutSequences);
};

END_CUTLEADER_NAMESPACE()
