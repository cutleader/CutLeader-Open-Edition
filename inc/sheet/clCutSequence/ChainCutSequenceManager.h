#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Line2D.h"
#include <map>
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(ChainNodeList)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(IMaterialSize);
DECLARE_CUTLEADER_CLASS(SequenceConfigItem);
DECLARE_CUTLEADER_CLASS(CrossPt_withGeomLoop)
DECLARE_CUTLEADER_CLASS(CrossPtList_withGeomLoop)
DECLARE_CUTLEADER_CLASS(ChainCutParam)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// 辅助类，记录直线穿过GreomLoop时的交点信息
class CrossPt_withGeomLoop
{
public:
	CrossPt_withGeomLoop(LONGLONG iPartInstID, const Point2D& pt_on_geomLoop)
	{
		m_iPartInstID = iPartInstID;
		m_pt_on_geomLoop = pt_on_geomLoop;
	}

	LONGLONG m_iPartInstID;
	Point2D m_pt_on_geomLoop;
};
class CrossPtList_withGeomLoop : public std::vector<CrossPt_withGeomLoopPtr>
{
public:
	CrossPtList_withGeomLoop() {}
	~CrossPtList_withGeomLoop() {}
};

class ClCutSequence_Export ChainCutSequenceManager
{
public:
	// 根据“连割点”和“回路实例”建立“连割工序”。
	static ChainCutSequencePtr BuildChainSeq(CutSequenceListPtr pCutSequences, Point2DListPtr pCutPts, LoopInstancePtr pLoopInstance);

	// 偏移“连割工序”。
	// 注：
	//   1) “零件放置”做了偏移之后，“连割工序”也需要做偏移。
	static ChainCutSequencePtr OffsetCutSequence(ChainCutSequencePtr pChainSeq, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// 得到所有的连割节点。
	static ChainNodeListPtr GetAllChainNode(CutSequenceListPtr pCutSequences);

	// 根据“连割节点”找到对应的工序。
	static ChainCutSequencePtr SearchChainSeq(CutSequenceListPtr pCutSequences, IChainNodePtr pChainNode);

	// 根据新的“零件实例”集合更新“连割工序”中的一些数据。
	// 注：
	//  1) 需要替换掉“回路实例”中的回路ID和“零件实例”。
	static void UpdateCutSequence(ChainCutSequencePtr pChainSeq, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

	// 判断这个连割工序是不是有效。
	// 注：
	//  1) 需要检测“连割工序”涉及的“回路实例”还在不在。
	//  2) 需要检测“连割工序”涉及的“回路实例”的切割方向是不是一样的。
	static BOOL IsCutSequenceInvalid(ChainCutSequencePtr pChainSeq, PartInstanceListPtr pAllPartInst);

	// 从零件外轮廓生成连割工序
	/* 参数：
    *  bNoSequenceExist: 生成这个工序前板子上是不是没有工序
	*  pLoopInsts_sameTopLayer: 这一拓扑层的所有零件外轮廓
	*  pLoopInsts_4_chainCut: 需要做连割的外轮廓
	*  outerLoopGeom: 板材上所有零件外轮廓的基本图形
	*/
	static ChainCutSequencePtr GenerateChainCutSequence(PartInstanceListPtr pPartInsts_offset, bool bNoSequenceExist, const Point2D& lastCutPt, LoopInstanceListPtr pLoopInsts_sameTopLayer,
        LoopInstanceListPtr pLoopInsts_4_chainCut, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const SequenceConfigItem* pSeqConfigItem);

	// 计算从点pt_from切到pLoopInstance的起始点，需要避让的路径。待完善，这个逻辑以后需要提到其他公共的类中。
	static LineArc2DListPtr Calc_avoidPath(const Point2D& pt_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary,
		PartInstanceListPtr pPartInsts_offset, const SequenceConfigItem* pSeqConfigItem);
	static LineArc2DListPtr Calc_avoidPath(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary,
		PartInstanceListPtr pPartInsts_offset, const SequenceConfigItem* pSeqConfigItem);

	// 把切割路径转换为连割节点。
	// 注：
	//  1) pChainCutPath是一些连在一起的path。
	//  2) bIgnoreFirstPt: 当第一个节点是直线时，是不是忽略掉起点
	static ChainNodeListPtr CutPath_2_ChainNodes(LineArc2DListPtr pChainCutPath, bool bIgnoreFirstPt = false);

    // 删除偏移了切割路径的复位点。
    // 注：
    //  1) pCutNodeList_4_simulate是板材上所有工序的切割路径
    //  2) 如果删除了复位点，需要把工序标记为修改
    static void DeleteResetPts_whichDeviateFromCutPath(CutSequenceListPtr pAllChainSeqs, CutNodeListPtr pCutNodeList_4_simulate);

private:
	// 计算在范围中的外轮廓，后续的连割路径是需要考虑这些外轮廓。这里面不包含from和to外轮廓。
	static std::map<LONGLONG, LineArc2DLoopPtr> CalcGeomLoops_inRange(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pLoopInsts_sameTopLayer,
																 std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoopData, ChainCutParamPtr pChainCutParam);

	// 计算在范围中的外轮廓，后续的连割路径是需要考虑这些外轮廓。这里面不包含to外轮廓。
	static std::map<LONGLONG, LineArc2DLoopPtr> CalcGeomLoops_inRange(const Point2D& point_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pLoopInsts_sameTopLayer,
		std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoopData, ChainCutParamPtr pChainCutParam);
	static LoopInstanceListPtr CalcLoopInsts_inRange(const Point2D& point_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary, ChainCutParamPtr pChainCutParam);
	static LoopInstanceListPtr CalcLoopInsts_inRange(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary, ChainCutParamPtr pChainCutParam);

	// 计算直线和LineArc2DLoop的交点<零件实例ID，交点>，并且按照到直线起点的距离排序。
	static CrossPtList_withGeomLoopPtr CalcCrossPts_with_geomLoops(const Line2D& line, const std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoop_inRange);

	// 计算出连割路径需要经过哪些零件。起始轮廓和终止轮廓之间的零件，<零件实例ID, 和这个外轮廓第一个交点，和这个外轮廓最后一个交点>。
	static std::vector<std::tr1::tuple<LONGLONG, Point2D, Point2D>> GetGeomLoopInfo_passed(CrossPtList_withGeomLoopPtr pCrossPtList_withGeomLoop,
		const Point2D& currentPt, const Point2D& endPt, const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real);

	// 切起始轮廓，需要返回最后的位置
	static LineArc2DListPtr CutStartGeomLoop(const LineArc2DLoop* pStartGeomLoop, const LineArc2DLoop* pStartGeomLoop_real, const Point2D& startPt, const Point2D& endPt, Point2D& currentPt);

	// 切“路过的”轮廓，需要返回最后的位置
	static LineArc2DListPtr CutPassedGeomLoop(std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoops_real, const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real,
		const Point2D& startPt, const Point2D& endPt, Point2D& currentPt);

	// 切终止轮廓
	static LineArc2DListPtr CutEndGeomLoop(const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real, const Point2D& startPt, const Point2D& endPt, const Point2D& currentPt);
};

// pCrossPt_withGeomLoop_1是不是在pCrossPt_withGeomLoop_2前面。这个函数用于对集合排序，靠pLine2D起点越近，就排前面。
bool IsPtBefore(CrossPt_withGeomLoopPtr pCrossPt_withGeomLoop_1, CrossPt_withGeomLoopPtr pCrossPt_withGeomLoop_2, Line2DPtr pLine2D);
bool IsPtBefore_1(const Point2D& pt_1, const Point2D& pt_2, Line2DPtr pLine2D);
bool IsPtBefore_2(const Point2D& pt_1, const Point2D& pt_2, const Point2D& pt);

END_CUTLEADER_NAMESPACE()
