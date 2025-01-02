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

// ���и�򡱹�������
class ClCutSequence_Export CutSequenceManager
{
public:
	// �����ʵ�����乤��
	/* ����:
	*  partRectMap: ���ʵ���и�����·ʵ������Ӿ��Ρ�
	*  pPartInstTopItems, partTopMap: the topology of the part inst in the sheet.
	*/
	static void AutoSequence(PartPlacementListPtr pPartPmts_offset, PartInstanceListPtr pPartInsts_offset,
		CutSequenceListPtr pAllCutSeqs, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG,Rect2D>>>& partRectMap,
		const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap,
		const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const ConfigData* pConfigData);

	// ɾ��һ������
	static void DeleteCutSeq(CutSequenceListPtr pCutSequences, const ICutSequence* pSeqUnit, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

	// ��ָ��λ��ǰ����һ�����и�򡱡�
	/* ����:
	*  iIndex: Ϊ-1�Ļ����ŵ�����档
	*/
	static void InsertCutSeq(CutSequenceListPtr pCutSequences, int iIndex, ICutSequencePtr pCutSequence, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

	// �ѹ����ƶ���ָ����λ��ǰ�档
	/* ����:
	*  iNewLoc: Ϊ-1�Ļ����Ƶ�����档
	*/
	static void MoveCutSeq(CutSequenceListPtr pCutSequences, int iNewLoc, ICutSequencePtr pCutSequence, PartInstanceListPtr pAllPartInst, const IMaterialSize* pMaterialSize,
		const SequenceConfigItem* pSeqConfigItem);

	// delete the invalid sequences.
	static bool DeleteInvalidSeq(CutSequenceListPtr pCutSequences, PartPlacementListPtr pAllPartPmts, PartInstanceListPtr pAllPartInst);
    // �õ���Ч���и��
    static CutSequenceListPtr GetInvalidSequences(CutSequenceListPtr pCutSequences, PartPlacementListPtr pAllPartPmts, PartInstanceListPtr pAllPartInst);

	// update the cache data in sequence.
	static void UpdateCache(CutSequenceListPtr pCutSequences);

	// �������ƫ�ơ�
	// ע��
	//   1) ������������µĹ������Ǻ��ϵ��Ƕ������ڴ棬����ͼ�α�ƫ�ƹ����滻�ˣ���������һ����
	//   2) �µġ����߹����б���ļ�������Ҫ��Ϊ0.
	static CutSequenceListPtr OffsetCutSequence(const CutSequenceList* pCutSequences, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// �����ͼ�����ϵĲο��㡣
	// ע��
	// 1) ���ڵ�ͼ���ú����ṩ��һЩ�ݴ������Ҳ����ο�������ʱ���ο�����Ϊ(0,0)����ʱ�����ûʲô���壬��ô���о����Ǻ���ȷ��
	//    bCannotFindRefLoopsOnCanvas�����������������������������������˵����ͼ��û��Ҫ�ӹ�����������ô����һ��������ĵ�Ҳ�޷�����Ϊ����Ҳ�ò�������㡣
	static Point2D CalcReferencePt(const PartPlacementList* pAllPartPlacements, const IMaterialSize* pMaterialSize, const RefPtData* pRefPtData,
		bool& bCannotFindRefLoopsOnCanvas);

private:
	// ����������˽ṹ�����ʵ�����乤��
	/* ����:
	*  lastCutPt: ���һ���г��㣬���������и���á��������һ������Ҳû��ʱ����������Ǹ��ݡ�������ʼλ�á��õ��ģ������Ǹ��ݲο���λ�õõ��ģ��ɿ����ο��������ĵ���
	*  pPrePartInst: ǰһ���ӹ��ġ����ʵ������
    *  pOldCutSeqs: ������֮ǰ�Ĺ���
    *  pNewCutSequences: �����ɵĹ���
	*/
    // ע��
    //  1) �ڴ�����Ƕ���ʱ������1�ǰ�һ�������������Ƕ�����ȫ���ӹ�����Ȼ����ȥ�и������������2���е�һ���ڿ׵���Ƕ���������ȥ�ӹ�����ڿס�
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
