#pragma once

#include "clCutSequenceCommon.h"
#include <map>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(LoopSequence)
DECLARE_CUTLEADER_CLASS(GridLoopSequence)
DECLARE_CUTLEADER_CLASS(SheetComCutSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(GridCutSequence)
DECLARE_CUTLEADER_CLASS(FlyCutSequence)
DECLARE_CUTLEADER_CLASS(SheetComCutGroupList)
DECLARE_CUTLEADER_CLASS(RemnantLineSeq)
DECLARE_CUTLEADER_CLASS(CanvasComCutSequence)
DECLARE_CUTLEADER_CLASS(CanvasComCutGroupList)
DECLARE_CUTLEADER_CLASS(RefPtData)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to save sequence unit data into database.
class ClCutSequence_Export SequenceDataLoader
{
public:
	// 加载底图或板材的参考点数据。
	static RefPtDataPtr LoadRefPtData(DBConnectPtr pDBConnect, bool dIsCanvas);

	// load sequence units of the sheet.
	/* params:
	*  pAllPartPmt: all part pmts.
	*  pAllPartInst: all part insts.
	*/
	// notes:
	//   1) after loading sequence, cache data in loop inst will be updated.
	static CutSequenceListPtr LoadSeqData(DBConnectPtr pDBConnect, PartPlacementListPtr pAllPartPmt, PartInstanceListPtr pAllPartInst);

private:
	/************************************************************************/
	// about loading loop sequence.

	// load loop sequence.
	/* params:
	*  pPartInstances: all loaded part inst list.
	*/
	// notes:
	//   1) we should check the loop inst exist, if the loop inst lost, return NULL.
	static LoopSequencePtr LoadLoopSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pPartInstances);
	static std::map<LONGLONG, LoopSequencePtr> LoadAllLoopSeq(DBConnectPtr pDBConnect, PartInstanceListPtr pPartInstances);

	// load loop grid sequence.
	/* params:
	*  pPartInstances: all loaded part inst list.
	*/
	// notes:
	//   1) we should check the loop inst exist, if the loop inst lost, return NULL.
	static GridLoopSequencePtr LoadLoopGridSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pPartInstances);
	/************************************************************************/


	/************************************************************************/
	// 关于板材共边工序。

	// 加载共边工序。
	static SheetComCutSequencePtr LoadComCutSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst, SheetComCutGroupListPtr pAllComCutGroup);

	// 加载所有共边组。
	// 注：
	//  1) 如果共边组中的零件实例都没有了，这个共边组就失效了。
	//  2) 把丢失的零件实例从共边组中删除。
	static SheetComCutGroupListPtr LoadAllComCutGroup(DBConnectPtr pDBConnect, PartInstanceListPtr pAllPartInst);
	/************************************************************************/


	/************************************************************************/
	// 关于底图共边工序。

	// 加载底图共边工序。
	static CanvasComCutSequencePtr LoadCanvasComCutSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst, CanvasComCutGroupListPtr pAllCanvasComCutGroup);

	// 加载所有底图共边组。
	// 注：
	//  1) 如果共边组中的轮廓实例都没有了，这个共边组就失效了。
	//  2) 把丢失的轮廓实例从共边组中删除。
	static CanvasComCutGroupListPtr LoadAllCanvasComCutGroup(DBConnectPtr pDBConnect, PartInstanceListPtr pAllPartInst);
	/************************************************************************/


	// 加载桥接工序。
	// 注：
	//  1) 加载时需要验证工序有效性。
	static BridgeSequencePtr LoadBdgeCutSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst);
	static BOOL LoadSubBdgeNode(DBConnectPtr pDBConnect, BridgeNodePtr pBridgeNode, PartInstanceListPtr pAllPartInst);

	// 加载连割工序。
	static ChainCutSequencePtr LoadChainSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst);

	// 加载田字工序。
	static GridCutSequencePtr LoadGridSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst);

	// 加载飞行切割工序。
	static FlyCutSequencePtr LoadFlyCutSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID, PartInstanceListPtr pAllPartInst);

	// load the trace points.
	static Point2DListPtr LoadTracePt(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID);
	static std::map<LONGLONG, Point2DListPtr> LoadAllTracePt(DBConnectPtr pDBConnect);

	// 加载“余料切割线”
	static RemnantLineSeqPtr LoadRemnantLineSeq(DBConnectPtr pDBConnect, LONGLONG iCutSequenceID);
};

END_CUTLEADER_NAMESPACE()
