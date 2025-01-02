#pragma once

#include "clCutSequenceCommon.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(RefPtData)

BEGIN_CUTLEADER_NAMESPACE()

// “切割工序”管理器。
class ClCutSequence_Export CutSequenceManager
{
public:
	// 给零件实例分配工序。
	/* 参数:
	*  partRectMap: 零件实例中各个回路实例的外接矩形。
	*  pPartInstTopItems, partTopMap: the topology of the part inst in the sheet.
	*/
	static void AutoSequence(PartPlacementListPtr pPartPmts_offset, PartInstanceListPtr pPartInsts_offset,
		CutSequenceListPtr pAllCutSeqs, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG,Rect2D>>>& partRectMap,
		const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap,
		const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// 删除一个工序。
	static void DeleteCutSeq(CutSequenceListPtr pCutSequences, const ICutSequence* pSeqUnit, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

	// 在指定位置前插入一个“切割工序”。
	/* 参数:
	*  iIndex: 为-1的话，放到最后面。
	*/
	static void InsertCutSeq(CutSequenceListPtr pCutSequences, int iIndex, ICutSequencePtr pCutSequence, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

	// 把工序移动到指定的位置前面。
	/* 参数:
	*  iNewLoc: 为-1的话，移到最后面。
	*/
	static void MoveCutSeq(CutSequenceListPtr pCutSequences, int iNewLoc, ICutSequencePtr pCutSequence, PartInstanceListPtr pAllPartInst, const IMaterialSize* pMaterialSize,
		const SequenceConfigItem* pSeqConfigItem);

	// delete the invalid sequences.
	static bool DeleteInvalidSeq(CutSequenceListPtr pCutSequences, PartPlacementListPtr pAllPartPmts, PartInstanceListPtr pAllPartInst);
    // 得到无效的切割工序。
    static CutSequenceListPtr GetInvalidSequences(CutSequenceListPtr pCutSequences, PartPlacementListPtr pAllPartPmts, PartInstanceListPtr pAllPartInst);

	// update the cache data in sequence.
	static void UpdateCache(CutSequenceListPtr pCutSequences);

	// 处理工序的偏移。
	// 注：
	//   1) 这个函数生成新的工序，它们和老的是独立的内存，里面图形被偏移过的替换了，其他数据一样。
	//   2) 新的“共边工序”中保存的激光宽度需要调为0.
	static CutSequenceListPtr OffsetCutSequence(const CutSequenceList* pCutSequences, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// 计算底图或板材上的参考点。
	// 注：
	// 1) 对于底图，该函数提供了一些容错处理，当找不到参考的轮廓时，参考点设为(0,0)，此时这个点没什么意义，这么做感觉不是很明确，
	//    bCannotFindRefLoopsOnCanvas表明发生了这种情况，如果遇到这种情况，说明底图上没有要加工的轮廓，那么返回一个无意义的点也无妨，因为后续也用不到这个点。
	static Point2D CalcReferencePt(const PartPlacementList* pAllPartPlacements, const IMaterialSize* pMaterialSize, const RefPtData* pRefPtData,
		bool& bCannotFindRefLoopsOnCanvas);

private:
	// 根据零件拓扑结构给零件实例分配工序。
	/* 参数:
	*  lastCutPt: 最后一个切出点，用来处理切割避让。当板材上一个工序也没有时，这个变量是根据“工序起始位置”得到的，而不是根据参考点位置得到的，可看看参考点的设计文档。
	*  pPrePartInst: 前一个加工的“零件实例”。
    *  pOldCutSeqs: 板子上之前的工序
    *  pNewCutSequences: 新生成的工序
	*/
    // 注：
    //  1) 在处理内嵌零件时，函数1是把一个零件的所有内嵌零件先全部加工掉，然后再去切父零件。而函数2是切掉一个内孔的内嵌零件后马上去加工这个内孔。
	static void SeqPartTopStruct_1(PartPlacementListPtr pPartPmts_offset, PartInstanceListPtr pPartInsts_offset,
        Point2D& lastCutPt, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG,Rect2D>>>& partRectMap,
        const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap,
        PartInstancePtr& pPrePartInst, LoopInstanceListPtr pSequencedLoopInsts, CutSequenceListPtr pNewCutSequences, const CutSequenceList* pOldCutSeqs,
        const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);
    static void SeqPartTopStruct_2(PartPlacementListPtr pPartPmts_offset, PartInstanceListPtr pPartInsts_offset,
        Point2D& lastCutPt, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG,Rect2D>>>& partRectMap,
        const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap,
        PartInstancePtr& pPrePartInst, LoopInstanceListPtr pSequencedLoopInsts, CutSequenceListPtr pNewCutSequences, const CutSequenceList* pOldCutSeqs,
        const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);
};

END_CUTLEADER_NAMESPACE()
