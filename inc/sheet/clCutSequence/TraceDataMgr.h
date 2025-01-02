#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(MachineParam)

BEGIN_CUTLEADER_NAMESPACE()

// ����·����������
class ClCutSequence_Export TraceDataMgr
{
public:
    // �����"lastCutPt"������ʼ��ı���·��(�����ƶ���)��
    static void ProcessAvoidPath(const Point2D& lastCutPt, ICutSequencePtr pCutSequence, const LoopInstanceList* pSeqLoopInsts, const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem);

    // ��˳��������Щ����֮��ı���·��
    // ע��
    //  1) pNewCutSeqList���Զ�����������ɵĹ��򣬶��Ҵ�ʱ���������ˡ��и���Զ������������ǻ�û��������·������������б����ж�����
    //  2) pOldCutSeqs��pNewCutSeqList֮ǰ����Щ���򣬵�Ȼ�����п���û��Ԫ�ء�
    static void ProcessAvoidPath(const CutSequenceList* pOldCutSeqs, CutSequenceListPtr pNewCutSeqList, const ConfigData* pConfigData);

	// ����������ݡ�
	static AvoidDataPtr BuildAvoidData(const IMaterialSize* pMaterialSize, const SequenceConfigItem* pSeqConfigItem, const MachineParam* pMachineParam);


	/************************************************************************/
	// ���ڼ�������ƶ��㡣

	// calculate the avoid points from "startPt" to "endPt".
	// notes:
	//   1) if no extra trace line is needed between "startPt" and "endPt", 0 item will be returned.
	//   2) if fail to avoid from "startPt" to "endPt", 0 item will be returned.
	static Point2DListPtr CalcAvoidPtList(const Point2D& startPt, const Point2D& endPt, const LoopInstanceList* pLoopInstList,
		AvoidDataPtr pAvoidData, BOOL& bFailAvoid);

	// calculate the avoid points from "startPt" to "endPt".
	/* params:
	*  pGeomLoopList: the loops should be avoided.
	*  rangeRect: �˶��㱻���Ƶķ�Χ��
	*/
	// notes:
	//   1) if no extra trace line is needed between "startPt" and "endPt", 0 item will be returned.
	//   2) if fail to avoid from "startPt" to "endPt", 0 item will be returned.
	static Point2DListPtr CalcAvoidPtList(const Point2D& startPt, const Point2D& endPt, LineArc2DLoopListPtr pGeomLoopList,
		const Rect2D& rangeRect, double dAvoidDis, BOOL& bFailAvoid);
	/************************************************************************/


	// �����startPt��endPt���ƶ���������ķ�Χ��
	static Rect2D GetAvoidPtRange(const Point2D& startPt, const Point2D& endPt, AvoidDataPtr pAvoidData);

	// get the loops which will be avoided.
	/* params:
	*  pSeqLoopInsts: the loop insts which have been sequenced, we should avoid them.
	*  rangeRect: the max rect range which contains the trace line path.
	*/
	// notes:
	//   1) we should filter "pSeqLoopInsts" using "rangeRect", you know, we only should avoid the loops which intersect with "rangeRect".
    //   2) ���������������������������Ҫ�ܿ�����������
	static LineArc2DLoopListPtr GetAvoidLoopList(const LoopInstanceList* pSeqLoopInsts, const Rect2D& rangeRect);

	// �õ����й���֮��Ŀ����ƶ��㡣
	// ע��
	//  1) ��Ӧĳ�����򱣴���Щ��Ϣ��<�ϸ�����Ľ�����, �м�ĵ�..., �������Ŀ�ʼ��>��
	static std::map<LONGLONG, Point2DListPtr> GetTracePtData(const Point2D& startCutPt, CutSequenceListPtr pCutSequences);
};

END_CUTLEADER_NAMESPACE()
