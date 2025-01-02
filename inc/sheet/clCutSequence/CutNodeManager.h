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

// ���и�ڵ㡱��������
class ClCutSequence_Export CutNodeManager
{
public:
	// �ѡ��и�򡱼���ת��Ϊ���и�ڵ㡱���ϡ�
	static CutNodeListPtr CutSequences2CutNodes(const Point2D& referencePt, const CutSequenceList* pCutSequences);
    // ����������ɵ��и�·����ģ��ʱ��һ���ģ����������������õ�����
    static CutNodeListPtr CutSequences2CutNodes_4_simulate(const Point2D& referencePt, const PartPlacementList* pPartPlacements, const PartInstanceList* pPartInstances,
        const CutSequenceList* pCutSequences, const LeadConfigItem* pLeadConfigItem, const SequenceConfigItem* pSeqConfigItem, std::set<LONGLONG>& patternLoops_failedToOffset);

	// �ӡ��и�ڵ㡱�õ���ģ����״����
	static SequenceSimulateShapeListPtr CutNode2SimShape(const ICutNode* pCutNode);
	static SequenceSimulateShapeListPtr CutNode2SimShape(const CutNodeList* pCutNodeList);

	// �ѡ��и�ڵ㡱��������ȫ����Ϊ�м��С�
	static void SetAllCutOn(CutNodeListPtr pCutNodeList);

	// �õ��ܵ�·�����ȡ�
	// ע��
	//  1) ����traceline��
	static double GetTotalPathLength(const CutNodeList* pCutNodes);

	// �õ��ܵ��и�ȡ�
	// ע��
	//  1) ������traceline��
	static double GetCutPathLength(const CutNodeList* pCutNodes);

	// �õ��ܵĿ��г̳��ȡ�
	static double GetRapidLength(const CutNodeList* pCutNodes);

	// ����̴�������
	static int GetPierceCount(const CutNodeList* pCutNodes);

private:
	// �ѡ��и��ת��Ϊ���и�ڵ㡱���ϡ�
	/* ������
	*  lastCutPt: the last hit point of the previous "do-cut" sequence and will be updated here.
	*/
	static CutNodeListPtr Sequence2CutNodes(Point2D& lastCutPt, ICutSequencePtr pCutSequence, BOOL bAvoidToNext, LoopInstanceListPtr pCutLoopInsts,
        std::map<LONGLONG, EdgeLoopListPtr>& edgeLoopsMap, std::map<LONGLONG, MicroJointDatasOfComCutGroupPtr>& microJointDataOfComCutGroups);


	/************************************************************************/
	// �ӡ��и�ڵ㡱�õ���ģ����״����

	// generate the simulation shape for cut line node.
	static CutSimulateShapePtr CutNode2SimShape(const CutLineNode* pCutLineNode);

	// generate the simulation shape for cut arc node.
	static CutSimulateShapePtr CutNode2SimShape(const CutArcNode* pCutArcNode);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
