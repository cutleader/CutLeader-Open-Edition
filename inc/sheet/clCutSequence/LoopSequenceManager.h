#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(LoopSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)

BEGIN_CUTLEADER_NAMESPACE()

// “回路工序”管理器。
class ClCutSequence_Export LoopSequenceManager
{
public:
	// 给“零件实例”中的“回路实例”分配工序。
	/* 参数:
	*  lastCutPt: 最后一个切出点，用来处理切割避让。
	*  pPreLoopInst: 前一个加工的回路实例。
	*  pPartInstTopItems: 当前拓扑层。
	*  pPartInstTopItem: 当前拓扑项。
	*  pLoopInstances: 要分配工序的“回路实例”，他们来自同一个“零件实例”。
	*  outerLoopGeom: 板材上所有“零件实例”外围回路的基本图形。
	*/
	// 注：
	// 1) 当共边等事先“截留”了外轮廓时，该函数就不会为零件实例外轮廓生成工序。
	static CutSequenceListPtr GenerateLoopSequences_4_partInstance(Point2D& lastCutPt, LoopInstancePtr pPreLoopInst,
		const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		LoopInstanceListPtr pLoopInstances, LoopInstanceListPtr pSeqLoopInsts, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// 给零件实例的内部“回路实例”分配工序。
	// 注：
	//  1) 以当前零件为一个范围来分配工序，即不考虑这个零件外的已有的工序，调用者需要重新处理避让关系。
	static CutSequenceListPtr GenLoopSeqs_4_innerLoops(CutSequenceListPtr pCutSequences, PartInstancePtr pPartInstance, PartInstanceListPtr pPartInstances, const ConfigData* pConfigData);

	// check whether loop sequence is invalid.
	// notes:
	//   1) if cannot find the loop inst in "pAllPartInst", the sequence is invalid.
	static BOOL IsCutSequenceInvalid(LoopSequencePtr pLoopSequence, PartInstanceListPtr pAllPartInst);

	// 偏移“回路工序”。
	// 注：
	//   1) “零件放置”做了偏移之后，“回路工序”也需要做偏移。
	static LoopSequencePtr OffsetCutSequence(LoopSequencePtr pLoopSequence, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// 根据新的“零件实例”集合更新工序中的一些数据。
	/* 参数：
	*  pOldPartInsts: 旧的零件实例集合。
	*  pNewPartInsts: 克隆的新的零件实例集合。
	*/
	// 注：
	//  1) 需要替换掉“回路实例”中的回路ID和“零件实例”。
	static void UpdateCutSequence(LoopSequencePtr pLoopSequence, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

    // 给回路实例分配工序。
    // 注：
    //  1) 这个函数需要更新最后的切出点和加工过的回路集合。
    static LoopSequencePtr BuildLoopSequence_underSheet(Point2D& lastHitPt, const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		LoopInstancePtr pLoopInstance, LoopInstanceListPtr pSeqLoopInsts, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, AvoidDataPtr pAvoidData, const ConfigData* pConfigData);
	static LoopSequencePtr BuildLoopSequence_underCanvas(Point2D& lastHitPt, LoopInstancePtr pLoopInstance, LoopInstanceListPtr pSeqLoopInsts,
		AvoidDataPtr pAvoidData, const ConfigData* pConfigData);

private:
	// when use cut avoidance, check whether need to disable lead out of the loop sequence.
	static void EnableLeadOut(LoopSequencePtr pLoopSequence);

	// 递归给零件实例的所有轮廓生成“轮廓工序”。这个递归函数不处理零件实例的外轮廓。
	static void GenerateLoopSequences(const LoopInstanceList* pLoopInstances, LoopInstanceListPtr pLoopInstances_sameLayer, CutSequenceListPtr pLoopSequences, Point2D& lastCutPt,
		LoopInstanceListPtr pSeqLoopInsts, LoopInstancePtr& pPreLoopInst, const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		AvoidDataPtr pAvoidData, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// 得到pLoopTopologyItem的下一层几何轮廓拓扑对象。
	// 注:
	// 1) 这个函数不仅仅是几何概念，如果一个轮廓的孩子轮廓不需要加工(设为不切或者已经被切过了)，那么会继续往下递归，直到拿到要加工(设为切割并且还没被切)的那个几何轮廓。
	//    所以如果pLoopTopologyItem的第一层孩子们都要加工的话，那么第一层孩子们就是需要返回的。
	// 2) pLoopInstances_uncut：当前做工序的零件实例中所有未加工的轮廓实例。
	static void GetChildLoopTopologyItems(const LoopTopologyItem* pLoopTopologyItem, const LoopCutFeatureList* pLoopCutFeatures, const LoopInstanceList* pLoopInstances_uncut,
		LoopTopologyItemListPtr pLoopTopologyItem_return);
};

END_CUTLEADER_NAMESPACE()
