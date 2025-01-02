#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutNode)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(EdgeLoopList)
DECLARE_CUTLEADER_CLASS(CutLineNode)
DECLARE_CUTLEADER_CLASS(CutArcNode)
DECLARE_CUTLEADER_CLASS(CutSimulateShape)
DECLARE_CUTLEADER_CLASS(CutNodeList)
DECLARE_CUTLEADER_CLASS(MicroJointDatasOfComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// “切割节点”管理器。
class ClCutSequence_Export CutNodeManager
{
public:
	// 把“切割工序”集合转化为“切割节点”集合。
	static CutNodeListPtr CutSequences2CutNodes(const Point2D& referencePt, const CutSequenceList* pCutSequences);
    // 这个函数生成的切割路径和模拟时是一样的，就是最终送往后置的坐标
    static CutNodeListPtr CutSequences2CutNodes_4_simulate(const Point2D& referencePt, const PartPlacementList* pPartPlacements, const PartInstanceList* pPartInstances,
        const CutSequenceList* pCutSequences, const LeadConfigItem* pLeadConfigItem, const SequenceConfigItem* pSeqConfigItem, std::set<LONGLONG>& patternLoops_failedToOffset);

	// 从“切割节点”得到“模拟形状”。
	static SequenceSimulateShapeListPtr CutNode2SimShape(const ICutNode* pCutNode);
	static SequenceSimulateShapeListPtr CutNode2SimShape(const CutNodeList* pCutNodeList);

	// 把“切割节点”的左右切全部改为中间切。
	static void SetAllCutOn(CutNodeListPtr pCutNodeList);

	// 得到总的路径长度。
	// 注：
	//  1) 包含traceline。
	static double GetTotalPathLength(const CutNodeList* pCutNodes);

	// 得到总的切割长度。
	// 注：
	//  1) 不包含traceline。
	static double GetCutPathLength(const CutNodeList* pCutNodes);

	// 得到总的空行程长度。
	static double GetRapidLength(const CutNodeList* pCutNodes);

	// 计算刺穿次数。
	static int GetPierceCount(const CutNodeList* pCutNodes);

private:
	// 把“切割工序”转化为“切割节点”集合。
	/* 参数：
	*  lastCutPt: the last hit point of the previous "do-cut" sequence and will be updated here.
	*/
	static CutNodeListPtr Sequence2CutNodes(Point2D& lastCutPt, ICutSequencePtr pCutSequence, BOOL bAvoidToNext, LoopInstanceListPtr pCutLoopInsts,
        std::map<LONGLONG, EdgeLoopListPtr>& edgeLoopsMap, std::map<LONGLONG, MicroJointDatasOfComCutGroupPtr>& microJointDataOfComCutGroups);


	/************************************************************************/
	// 从“切割节点”得到“模拟形状”。

	// generate the simulation shape for cut line node.
	static CutSimulateShapePtr CutNode2SimShape(const CutLineNode* pCutLineNode);

	// generate the simulation shape for cut arc node.
	static CutSimulateShapePtr CutNode2SimShape(const CutArcNode* pCutArcNode);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
