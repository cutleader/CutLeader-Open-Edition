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

// ����·���򡱹�������
class ClCutSequence_Export LoopSequenceManager
{
public:
	// �������ʵ�����еġ���·ʵ�������乤��
	/* ����:
	*  lastCutPt: ���һ���г��㣬���������и���á�
	*  pPreLoopInst: ǰһ���ӹ��Ļ�·ʵ����
	*  pPartInstTopItems: ��ǰ���˲㡣
	*  pPartInstTopItem: ��ǰ�����
	*  pLoopInstances: Ҫ���乤��ġ���·ʵ��������������ͬһ�������ʵ������
	*  outerLoopGeom: ��������С����ʵ������Χ��·�Ļ���ͼ�Ρ�
	*/
	// ע��
	// 1) �����ߵ����ȡ���������������ʱ���ú����Ͳ���Ϊ���ʵ�����������ɹ���
	static CutSequenceListPtr GenerateLoopSequences_4_partInstance(Point2D& lastCutPt, LoopInstancePtr pPreLoopInst,
		const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		LoopInstanceListPtr pLoopInstances, LoopInstanceListPtr pSeqLoopInsts, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// �����ʵ�����ڲ�����·ʵ�������乤��
	// ע��
	//  1) �Ե�ǰ���Ϊһ����Χ�����乤�򣬼�������������������еĹ��򣬵�������Ҫ���´�����ù�ϵ��
	static CutSequenceListPtr GenLoopSeqs_4_innerLoops(CutSequenceListPtr pCutSequences, PartInstancePtr pPartInstance, PartInstanceListPtr pPartInstances, const ConfigData* pConfigData);

	// check whether loop sequence is invalid.
	// notes:
	//   1) if cannot find the loop inst in "pAllPartInst", the sequence is invalid.
	static BOOL IsCutSequenceInvalid(LoopSequencePtr pLoopSequence, PartInstanceListPtr pAllPartInst);

	// ƫ�ơ���·���򡱡�
	// ע��
	//   1) ��������á�����ƫ��֮�󣬡���·����Ҳ��Ҫ��ƫ�ơ�
	static LoopSequencePtr OffsetCutSequence(LoopSequencePtr pLoopSequence, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// �����µġ����ʵ�������ϸ��¹����е�һЩ���ݡ�
	/* ������
	*  pOldPartInsts: �ɵ����ʵ�����ϡ�
	*  pNewPartInsts: ��¡���µ����ʵ�����ϡ�
	*/
	// ע��
	//  1) ��Ҫ�滻������·ʵ�����еĻ�·ID�͡����ʵ������
	static void UpdateCutSequence(LoopSequencePtr pLoopSequence, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

    // ����·ʵ�����乤��
    // ע��
    //  1) ���������Ҫ���������г���ͼӹ����Ļ�·���ϡ�
    static LoopSequencePtr BuildLoopSequence_underSheet(Point2D& lastHitPt, const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		LoopInstancePtr pLoopInstance, LoopInstanceListPtr pSeqLoopInsts, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, AvoidDataPtr pAvoidData, const ConfigData* pConfigData);
	static LoopSequencePtr BuildLoopSequence_underCanvas(Point2D& lastHitPt, LoopInstancePtr pLoopInstance, LoopInstanceListPtr pSeqLoopInsts,
		AvoidDataPtr pAvoidData, const ConfigData* pConfigData);

private:
	// when use cut avoidance, check whether need to disable lead out of the loop sequence.
	static void EnableLeadOut(LoopSequencePtr pLoopSequence);

	// �ݹ�����ʵ���������������ɡ��������򡱡�����ݹ麯�����������ʵ������������
	static void GenerateLoopSequences(const LoopInstanceList* pLoopInstances, LoopInstanceListPtr pLoopInstances_sameLayer, CutSequenceListPtr pLoopSequences, Point2D& lastCutPt,
		LoopInstanceListPtr pSeqLoopInsts, LoopInstancePtr& pPreLoopInst, const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem,
		AvoidDataPtr pAvoidData, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// �õ�pLoopTopologyItem����һ�㼸���������˶���
	// ע:
	// 1) ��������������Ǽ��θ�����һ�������ĺ�����������Ҫ�ӹ�(��Ϊ���л����Ѿ����й���)����ô��������µݹ飬ֱ���õ�Ҫ�ӹ�(��Ϊ�и�һ�û����)���Ǹ�����������
	//    �������pLoopTopologyItem�ĵ�һ�㺢���Ƕ�Ҫ�ӹ��Ļ�����ô��һ�㺢���Ǿ�����Ҫ���صġ�
	// 2) pLoopInstances_uncut����ǰ����������ʵ��������δ�ӹ�������ʵ����
	static void GetChildLoopTopologyItems(const LoopTopologyItem* pLoopTopologyItem, const LoopCutFeatureList* pLoopCutFeatures, const LoopInstanceList* pLoopInstances_uncut,
		LoopTopologyItemListPtr pLoopTopologyItem_return);
};

END_CUTLEADER_NAMESPACE()
