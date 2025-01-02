#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)


BEGIN_CUTLEADER_NAMESPACE()


// “桥接工序”管理器。
class ClCutSequence_Export BridgeSequenceManager
{
public:
	/************************************************************************/
	// 关于创建“桥接工序”。

	// 根据2个“回路实例”建立“桥接工序”。
	static BridgeSequencePtr BuildBdgeSeq(LoopInstancePtr pFstLoopInst, BridgePtPtr pFstBdgePt, LoopInstancePtr pSndLoopInst, BridgePtPtr pSndBdgePt, double dBridgeWidth);

	// 根据“回路实例”上的点来构建“桥接点”。
	static BridgePtPtr BuildBdgePt(LoopInstancePtr pLoopInstance, int iLoopNodeIndex, const Point2D& bdgePt);

	// 判断在这2个“图形实例”的指定的位置能不能建立桥接。
	// 注：
	//   1) 当桥的宽度足够小时，总是能建立桥接的。
	//   2) 当在角上桥接时，桥接点位置可能被调整。
	static BOOL TwoLoopCanBdge(LoopInstancePtr pFstLoopInst, int& iFstLoopNodeIndex, Point2D& fstBdgePt, LoopInstancePtr pSndLoopInst, int& iSndLoopNodeIndex, Point2D& sndBdgePt,
							   double dBdgeWidth);

	// 检查这2个“图形实例”间能不能桥接。
	// 注：
	//  1) 当这2个“回路实例”上都有“桥接工序”，且是同一个“桥接工序”时，不能桥接它们。
	static BOOL TwoLoopCanBdge(CutSequenceListPtr pCutSequences, const LoopInstance* pFstLoopInst, const LoopInstance* pSndLoopInst);
	/************************************************************************/


	/************************************************************************/
	// 关于得到桥接的基本图形（板材坐标系下）。

	// 得到“桥接节点”的某一个“出口桥接点”所在的“基本图形”（板材坐标系下）。
	// 注：
	//   1) “基本图形”的方向遵循切割方向。
	static ILineArc2DPtr GetOutBdgePtGeom(BridgeNodePtr pBridgeNode, int iBdgePtIdx);
	static ILineArc2DPtr GetOutBdgePtGeom(BridgeNodePtr pBridgeNode, BridgePtPtr pBdgePt);

	// 得到“桥接节点”的“入口桥接点”所在的“基本图形”（板材坐标系下）。
	// 注：
	//   1) “基本图形”的方向遵循切割方向。
	static ILineArc2DPtr GetInBdgePtGeom(BridgeNodePtr pBridgeNode);

	// 得到所有“桥接节点”及它的“入口桥”的基本图形。
	// 注：
	//  1) “根节点”不返回。因为其没有“入口桥”。
	static std::vector<std::pair<BridgeNodePtr, LineArc2DListPtr> > GetAllBdgeGeomInfo(CutSequenceListPtr pCutSequences);

	// 从这个父亲节点向下递归，得到每个桥的基本图形。
	// 注：
	//   1) 其他函数调用这个函数时，需注意“pBridgeNode”不能是叶子节点。
	//   2) 这是个递归函数。
	static void GetBdgeGeomInfo(BridgeNodePtr pBridgeNode, std::vector<std::pair<BridgeNodePtr, LineArc2DListPtr>>& bdgeGeomInfo);

	// 得到“叶子桥接节点”的“入口桥”的基本图形。
	static LineArc2DListPtr GetBdgeGeom(BridgeNodePtr pSubBdgeNode);
	/************************************************************************/


	/************************************************************************/
	// 关于得到“桥接点”或“桥点”坐标。

	// 得到“桥接点”坐标。
	static Point2D GetBdgePosition(LoopInstancePtr pLoopInstance, BridgePtPtr pBridgePt);

	// 得到“桥接节点”的某个“出口桥接点”的2个“桥点”坐标。
	/* 参数：
	*  iBdgePtIdx: “出口桥接点”索引。
	*/
	// 注：
	//   1) 这2个“桥点”按照实际切割顺序排序。
	static Point2DListPtr GetTwoOutBdgePt(BridgeNodePtr pBridgeNode, int iBdgePtIdx);

	// 得到“桥接节点”的“入口桥接点”的2个“桥点”坐标。
	/* 参数：
	*  parentOutBdgePt: “父亲桥接节点”对应的“出口桥接点”坐标。
	*/
	// 注：
	//   1) 这2个“桥点”按照实际切割顺序排序。
	static Point2DListPtr GetTwoInBdgePt(Point2D parentOutBdgePt, BridgeNodePtr pBridgeNode);
	/************************************************************************/


	// 把这个节点的“父节点”设为自己的“子节点”。
	// 注：
	//  1) 这个函数需要递归到根节点，这样这个节点就变成了根节点。
	static void SetAsParent(BridgeNodePtr pBridgeNode);

	// 从这个“桥接节点”搜索“根节点”。
	static BridgeNodePtr GetRootBdgeNode(BridgeNodePtr pBridgeNode);

	// 偏移“桥接工序”。
	// 注：
	//   1) “零件放置”做了偏移之后，“桥接工序”也需要做偏移。
	static BridgeSequencePtr OffsetCutSequence(BridgeSequencePtr pBridgeSeq, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// 根据新的“零件实例”集合更新“桥接工序”中的一些数据。
	/* 参数：
	*  pOldPartInsts: 旧的零件实例集合。
	*  pNewPartInsts: 克隆的新的零件实例集合。
	*/
	// 注：
	//  1) 需要替换掉“回路实例”中的回路ID和“零件实例”。
	//  2) 需要替换掉“桥接点”中的图形ID。
	//  3) 这是个递归函数。
	static void UpdateCutSequence(BridgeNodePtr pBridgeNode, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

	// 判断这个桥接工序是不是有效。
	// 注：
	//  1) 需要检测“桥接工序”涉及的“回路实例”还在不在。
	//  2) 需要检测“桥接工序”涉及的“回路实例”的切割方向是不是一样的。
	static BOOL IsCutSequenceInvalid(BridgeSequencePtr pBridgeSeq, PartInstanceListPtr pAllPartInst);

private:
	// 替换掉所有“桥接节点”中“回路实例”对象中的“零件实例”对象。
	// 注：
	//  1) 这是个递归函数。
	static void ReplacePartInst(BridgeNodePtr pBridgeNode, PartInstanceListPtr pPartInstances);

	// 返回两个“角岸”和一个“边岸”之间的搭桥方式。
	/* 参数：
	*  pCornerDock1, pCornerDock2: 两个“角岸”。
	*  pEdgeDock: “边岸”。
	*  dockPt: “边岸”上面的桥接点。
	*  bCCW: 为真，桥的第一条边通过“参考线”逆时针旋转得到。否则顺时针旋转得到。
	*  retPt: 返回的某一个“角岸”上的桥接点。
	*/
	// 注：
	//  1) 这个函数返回一个搭桥方案，桥的第一条边的确定有两种方式，每种方式确定一个搭桥方法。
	static BOOL GetCornerBdgePt(ILineArc2DPtr pCornerDock1, ILineArc2DPtr pCornerDock2, const Point2D& cornerPt, ILineArc2DPtr pEdgeDock, const Point2D& dockPt,
		double dBdgeWid, BOOL bCCW,	Point2D& retPt);

	// 返回两个“角岸”和另外两个“角岸”之间的搭桥方式。
	/* 参数：
	*/
	// 注：
	//  1) 两个“角岸”之间的架桥方式有两种：
	//     a) 两个角点连起来的架桥方式。
	//     b) 错开的架桥方式。
	static BOOL GetCornerBdgePt(ILineArc2DPtr pCornerDock1, const Point2D& cornerPt1, ILineArc2DPtr pCornerDock2, const Point2D& cornerPt2, double dBdgeWid, Point2D& retPt1, Point2D& retPt2);
};

END_CUTLEADER_NAMESPACE()
