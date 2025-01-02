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

// �����࣬��¼ֱ�ߴ���GreomLoopʱ�Ľ�����Ϣ
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
	// ���ݡ�����㡱�͡���·ʵ��������������򡱡�
	static ChainCutSequencePtr BuildChainSeq(CutSequenceListPtr pCutSequences, Point2DListPtr pCutPts, LoopInstancePtr pLoopInstance);

	// ƫ�ơ�����򡱡�
	// ע��
	//   1) ��������á�����ƫ��֮�󣬡������Ҳ��Ҫ��ƫ�ơ�
	static ChainCutSequencePtr OffsetCutSequence(ChainCutSequencePtr pChainSeq, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// �õ����е�����ڵ㡣
	static ChainNodeListPtr GetAllChainNode(CutSequenceListPtr pCutSequences);

	// ���ݡ�����ڵ㡱�ҵ���Ӧ�Ĺ���
	static ChainCutSequencePtr SearchChainSeq(CutSequenceListPtr pCutSequences, IChainNodePtr pChainNode);

	// �����µġ����ʵ�������ϸ��¡�������е�һЩ���ݡ�
	// ע��
	//  1) ��Ҫ�滻������·ʵ�����еĻ�·ID�͡����ʵ������
	static void UpdateCutSequence(ChainCutSequencePtr pChainSeq, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

	// �ж����������ǲ�����Ч��
	// ע��
	//  1) ��Ҫ��⡰������漰�ġ���·ʵ�������ڲ��ڡ�
	//  2) ��Ҫ��⡰������漰�ġ���·ʵ�������и���ǲ���һ���ġ�
	static BOOL IsCutSequenceInvalid(ChainCutSequencePtr pChainSeq, PartInstanceListPtr pAllPartInst);

	// ��������������������
	/* ������
    *  bNoSequenceExist: �����������ǰ�������ǲ���û�й���
	*  pLoopInsts_sameTopLayer: ��һ���˲���������������
	*  pLoopInsts_4_chainCut: ��Ҫ�������������
	*  outerLoopGeom: �������������������Ļ���ͼ��
	*/
	static ChainCutSequencePtr GenerateChainCutSequence(PartInstanceListPtr pPartInsts_offset, bool bNoSequenceExist, const Point2D& lastCutPt, LoopInstanceListPtr pLoopInsts_sameTopLayer,
        LoopInstanceListPtr pLoopInsts_4_chainCut, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const SequenceConfigItem* pSeqConfigItem);

	// ����ӵ�pt_from�е�pLoopInstance����ʼ�㣬��Ҫ���õ�·���������ƣ�����߼��Ժ���Ҫ�ᵽ�������������С�
	static LineArc2DListPtr Calc_avoidPath(const Point2D& pt_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary,
		PartInstanceListPtr pPartInsts_offset, const SequenceConfigItem* pSeqConfigItem);
	static LineArc2DListPtr Calc_avoidPath(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary,
		PartInstanceListPtr pPartInsts_offset, const SequenceConfigItem* pSeqConfigItem);

	// ���и�·��ת��Ϊ����ڵ㡣
	// ע��
	//  1) pChainCutPath��һЩ����һ���path��
	//  2) bIgnoreFirstPt: ����һ���ڵ���ֱ��ʱ���ǲ��Ǻ��Ե����
	static ChainNodeListPtr CutPath_2_ChainNodes(LineArc2DListPtr pChainCutPath, bool bIgnoreFirstPt = false);

    // ɾ��ƫ�����и�·���ĸ�λ�㡣
    // ע��
    //  1) pCutNodeList_4_simulate�ǰ�������й�����и�·��
    //  2) ���ɾ���˸�λ�㣬��Ҫ�ѹ�����Ϊ�޸�
    static void DeleteResetPts_whichDeviateFromCutPath(CutSequenceListPtr pAllChainSeqs, CutNodeListPtr pCutNodeList_4_simulate);

private:
	// �����ڷ�Χ�е�������������������·������Ҫ������Щ�������������治����from��to��������
	static std::map<LONGLONG, LineArc2DLoopPtr> CalcGeomLoops_inRange(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pLoopInsts_sameTopLayer,
																 std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoopData, ChainCutParamPtr pChainCutParam);

	// �����ڷ�Χ�е�������������������·������Ҫ������Щ�������������治����to��������
	static std::map<LONGLONG, LineArc2DLoopPtr> CalcGeomLoops_inRange(const Point2D& point_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pLoopInsts_sameTopLayer,
		std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoopData, ChainCutParamPtr pChainCutParam);
	static LoopInstanceListPtr CalcLoopInsts_inRange(const Point2D& point_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary, ChainCutParamPtr pChainCutParam);
	static LoopInstanceListPtr CalcLoopInsts_inRange(LoopInstancePtr pLoopInst_from, LoopInstancePtr pLoopInst_to, LoopInstanceListPtr pAllLoopInsts_boundary, ChainCutParamPtr pChainCutParam);

	// ����ֱ�ߺ�LineArc2DLoop�Ľ���<���ʵ��ID������>�����Ұ��յ�ֱ�����ľ�������
	static CrossPtList_withGeomLoopPtr CalcCrossPts_with_geomLoops(const Line2D& line, const std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoop_inRange);

	// ���������·����Ҫ������Щ�������ʼ��������ֹ����֮��������<���ʵ��ID, �������������һ�����㣬��������������һ������>��
	static std::vector<std::tr1::tuple<LONGLONG, Point2D, Point2D>> GetGeomLoopInfo_passed(CrossPtList_withGeomLoopPtr pCrossPtList_withGeomLoop,
		const Point2D& currentPt, const Point2D& endPt, const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real);

	// ����ʼ��������Ҫ��������λ��
	static LineArc2DListPtr CutStartGeomLoop(const LineArc2DLoop* pStartGeomLoop, const LineArc2DLoop* pStartGeomLoop_real, const Point2D& startPt, const Point2D& endPt, Point2D& currentPt);

	// �С�·���ġ���������Ҫ��������λ��
	static LineArc2DListPtr CutPassedGeomLoop(std::map<LONGLONG, LineArc2DLoopPtr>& outerGeomLoops_real, const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real,
		const Point2D& startPt, const Point2D& endPt, Point2D& currentPt);

	// ����ֹ����
	static LineArc2DListPtr CutEndGeomLoop(const LineArc2DLoop* pEndGeomLoop, const LineArc2DLoop* pEndGeomLoop_real, const Point2D& startPt, const Point2D& endPt, const Point2D& currentPt);
};

// pCrossPt_withGeomLoop_1�ǲ�����pCrossPt_withGeomLoop_2ǰ�档����������ڶԼ������򣬿�pLine2D���Խ��������ǰ�档
bool IsPtBefore(CrossPt_withGeomLoopPtr pCrossPt_withGeomLoop_1, CrossPt_withGeomLoopPtr pCrossPt_withGeomLoop_2, Line2DPtr pLine2D);
bool IsPtBefore_1(const Point2D& pt_1, const Point2D& pt_2, Line2DPtr pLine2D);
bool IsPtBefore_2(const Point2D& pt_1, const Point2D& pt_2, const Point2D& pt);

END_CUTLEADER_NAMESPACE()
