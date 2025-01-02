#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "baseConst.h"
#include "Vector2D.h"
#include <tuple>
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(PierceConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ���е�����ķ�ʽ��
enum MOV_START_CUT_TYPE
{
	GEOM_CORNER_2_CORNER	= 0, // ��ͼ�λ�ǵ��������ϡ�
	CORNER_2_GEOM			= 1, // �ӽǵ�����ͼ���ϡ�
	GEOM_2_GEOM				= 2, // ��ͼ�ε�����ͼ���ϡ�
	CANNOT_MOVE				= 3, // ���ܵ������µ����е�λ�ú��ϵ�һ��������180�ȵĽǴ�����
};


// ����·������������������
class ClCutFeature_Export LoopStartCutProcessor
{
public:
	/************************************************************************/
	// ����·�����������ߺ�ͼ�εĸ��档

	// ����ڲ���·�����������ߺ�ͼ�εĸ��档
	// ע��:
	//   1) ���������߲������������
	//      a) ���������ߵ���Χ���β��ô��ڻ�·��Χ���ε�һ�롣
	//      b) �̴�������ڻ�·�ڲ���
	//      c) ���������ߺͱ���·�ĸ�������һ����Χ�ڡ�
	//   2) this function should update the cache data of loop start/end cut object.
	//   3) ��������:
	//        ����1: �Ȱ����������߳��ȿ��Ƶ���·�����һ�롣
	//        ����2: Ҫ�ǻ��Ǹ��棬�����е㻻����λ����һ�ԡ�
	//        ����3: Ҫ�ǻ��Ǹ��棬���������������̡�
	//        ����4: ��ʱҪ�ǻ����棬�ظ�����2,3 N�Ρ�
	static void CheckInnerLoopIntf(LoopStartCutDataPtr pLoopStartCutData, const LoopTopologyItemList* pLoopTopologyItems, double dAllowIntfArea = GEOM_TOLERANCE);

	// ����ⲿ��·�����������ߺ�ͼ�εĸ��档
	// ע��:
	//   1) �ⲿ��·�����������л�·��
	//   2) ���������߲������������
	//      a) ���������ߵ���Χ���β��ô��ڻ�·��Χ���ε�һ�롣
	//   3) this function should update the cache data of loop start/end cut object.
	static void CheckOuterLoopIntf(LoopStartCutDataPtr pLoopStartCutData, double dAllowIntfArea = GEOM_TOLERANCE);

	// ��·�����������ߺ�ͼ���Ƿ���档
	// ע:
	//   1) ���������߸����������
	//		a) ��Ҫ����������ƫ�Ʒ��򣬼��̴����ڼ��������ڻ��Ǽ��������⣬������ƫ�ƣ���̴����ڼ��������ڲźϷ���
	//      b) ���������߲��ܱ������ضϣ�������������ߵĶ˵�Ӵ�������������Ϊ�ǽضϡ�
	static BOOL IsLoopIntf(const LoopStartCutData* pLoopStartCutData);
	/************************************************************************/


	/************************************************************************/
	// ���������С�

	// �л�����Ϊ���л����У��ú������������ߡ�
	// ע��
	// 1) ֻ�ܶԷ������ִ����Щ������
	// 2) ��Щ����ʹ���˻��棬��û�и��»��档
	static void CutOutsideLoop(LoopStartCutDataPtr pLoopStartCutData);
	static void CutInsideLoop(LoopStartCutDataPtr pLoopStartCutData);

	// �л��������Լ�cut-on��˳��Ϊ��--on--�ڣ�����ѭ����
	// �޸������������з�ʽ��
	// ע��
	// 1) ���øú���ǰҪȷ�ϴ�������������޸���������ΪemPatternLoopCutOffsetType������ú�����debugWarn������false��
	//    ���粻�ܴ��뿪�����������ܰ�һ�����е�������Ϊ���С�
	static bool ChangeLoopCutSide(LoopStartCutDataPtr pLoopStartCutData, PatternLoopCutOffsetType emCutOffsetType, const LeadConfigItem* pLeadConfigItem);

	// �жϱպϻ�·�������л��������С�
	static PatternLoopCutOffsetType CalculateLoopCutOffset(const LoopStartCutData* pLoopStartCutData);

	// Ϊ������������и�ƫ�Ʒ�ʽ��û�и��»��档
	static void SetLoopCutOffset_4_closedLoop(LoopStartCutDataPtr pLoopStartCutData, const LoopTopologyItemList* pLoopTopologyItems);
	/************************************************************************/


	/************************************************************************/
	// �������е㣬�⼸������û�з���StartCutPtProcessor�С�

	// �������е��λ�á�
	/* ������
	*  iNodeIndex: �µ����е����ڡ���·�ڵ㡱��
	*  startPt: �µ����е㡣
	*/
	// ע��
	//   1) ��Ҫ��������֮ǰ�����������߳���(��ʹ����)�����ǵ����е�ӽ��ϵ�����ͼ����ʱ��
	//      ���������߳�������ר�ҿ⣬��ʱ���ܷ�������������߳������㣬����������Ͳ�������ԭ���ĳ����ˡ�
	//   2) ��Ҫ��������֮ǰ���и�������
	//   3) ��Ҫ���»��档
	//   4) ��Щ������ܵ�����
	static void ChangeStartCutPt(LoopStartCutDataPtr pLoopStartCutData, int iNodeIndex, const Point2D& startPt, const LeadConfigItem* pLeadConfigItem);

	// �������е�ķ�ʽ��
	// ע��
	//  1) �е�����²��ܵ�������newStartPt����
	static MOV_START_CUT_TYPE StartCutPtMovType(LoopStartCutDataPtr pLoopStartCutData, const Point2D& newStartPt);

    // get the "position" info of the start cut point.
    // notes:
    //   1) if the start cut point is at corner, we need to figure out which side will be the start one.
    static void GetStartPtInfo(const LoopStartCutData* pLoopStartCutData, LONGLONG& iStartPatID, PatternPosition& startPatPos);
	/************************************************************************/


	/************************************************************************/
	// ����������������������
	// ע��
	// 1) bUsed_cutOnTool: ������������ǲ�������cut-on�ĵ��ߡ����ֵ��Ҫ�������ͽ�����ԭ����LoopToolData��û�м�¼���ߵ�׼ȷ��Ϣ����Ȼ��¼�˵������ƣ�
	//    �����ڱ�������ʱ��������ȥ���ҵ��߲��ɿ�������Ŀǰֻ�������������ͽ�����

	// ���������䡰��·��������������������������Ϊ���ü��������ġ���·������������
	// ע:
	// 1) this function will init start/end cut info, so do not use cache data in this function.
	// 2) this function should update the cache data after init the start/end cut object.
	// 3) �����������ʱ�������Ͽ��ܻ�û�з��䡰��·������������
	static void AssignLoopStaEndCut(LoopStartCutDataPtr pLoopStartCutData, const LoopTopologyItemList* pLoopTopologyItems, const LeadConfigItem* pLeadConfigItem,
		const PierceConfigItem* pPierceConfigItem, bool bUsed_cutOnTool, const Matrix2D& transformMatInSheet);

	// ���ü��������ϵ����ߡ������������ʱ���������Ѿ����ˡ���·������������
	// ע��
	// 1) ����������ı伸�����������е���������з�ʽ��
	// 2) ����������ж�������Ҫ��Ҫ���ߣ�Ȼ����������ͼ���ϻ����ڽ����Լ��������з�ʽ����ר�ҿ��ȡ��Ӧ�Ĳ������½������߷��䡣
	static void ResetLeadInfoOnPatternLoop(LoopStartCutDataPtr pLoopStartCutData, const LoopTopologyItemList* pLoopTopologyItems,
		const LeadConfigItem* pLeadConfigItem);

	// ���ü������������е㡣�����������ʱ���������Ѿ����ˡ���·������������
	// ע��
	// 1) ����������ı伸���������������з�ʽ�����ܻ�ı����ߣ��������е�ӱ��ϱ��Ƶ��˽��ϣ���
	static void ResetStartCutPtOnPatternLoop(LoopStartCutDataPtr pLoopStartCutData, const LoopTopologyItemList* pLoopTopologyItems, const LeadConfigItem* pLeadConfigItem,
		const PierceConfigItem* pPierceConfigItem, const Matrix2D& transformMatInSheet);
	/************************************************************************/


	/************************************************************************/
	// �����������и��

	// reverse the cut direction of the loop.
	// ע:
	//   1) for the lead in corner, we need to exchange its lead in/out.
	//   2) ��Ҫ���»��档
	static void ReverseCutDir(LoopStartCutDataPtr pLoopStartCutData);

	// �õ�����������и��
	// ע��
	// 1) ���øú���ʱ��ȷ�ϴ���������Ƿ�յġ�
	static DIRECTION GetClosedLoopCutDirection(const LoopStartCutData* pLoopStartCutData);
	/************************************************************************/


	// ��顰��·�����������ǲ�����Ч��
	// ע:
	//   1) if the start/end cut point is at geom before process, it will be not available when:
	//         a) the "start" pattern is lost.
	//         b) the loop has been open.
	//   2) if the start/end cut point is at the ends of open path before process, it will be not available when:
	//         a) the "start" pattern is lost.
	//         b) the "start" pattern is not at the ends of the loop.
	//         c) the loop has been close.
	//   3) if the start/end cut point is at corner before process, it will be not available when:
	//         a) two adjacent patterns are NOT connected as they did before.
	//		   b) the loop has been open.
	//      in this case, even if the a), b) items did not happened, we still should refine the start/end cut info for some cases, e.g. the corner angle was changed.
	//   4) ����������û��档
	static BOOL IsAvailable(LoopStartCutDataPtr pLoopStartCutData);

	// ����·ƫ�ƺ�ġ���·������������
	// ע��
	//  1) ֻ����պϵĶ���λ�·��
	//  2) ���ԭ�������е���ͼ���ϣ�ƫ��֮�����ڵ�ͼ�ζ�ʧ�ˣ�������е���䵽����Ľ��ϡ�
	//  3) ���ԭ�������е��ڽ��ϣ�
	//     a) ƫ��֮����ϵ�����ͼ�ζ���һ�����߶����ˣ�������е���䵽����Ľ��ϡ�
	//     b) ƫ��֮����˵��ǣ���ʱ�����е��Ƶ�����������ϡ�
	static void Check4Offset(LoopStartCutDataPtr pLoopStartCutData);

	// calc the un-pierce point.
	// notes:
	//   1) if array loop and bArrayLastInst is TRUE, just for last inst.
	//   2) this function will use the cache of start/end cut data.
	static Point2D CalcUnPiercePt(const LoopStartCutData* pLoopStartCutData, BOOL bArrayLastInst = FALSE);

	// get the loop that it's pierce point is closest to "pt".
	// notes:
	//   1) this function will use the cache of start/end cut data.
	static IPatternLoopPtr GetClosestLoop(LoopStartCutDataListPtr pLoopStaEndCutList, Point2D pt);

	// update the cache data of loop start/end cut object.
	static void UpdateCache(LoopStartCutDataPtr pLoopStartCutData);
	static void UpdateCache(LoopStartCutDataListPtr pLoopStaEndCutList);

private:
	/************************************************************************/
	// get pierce/un-pierce pt for all type lead.

	// calc the pierce point.
	// notes:
	//   1) if array loop, just for first inst.
	//   2) call this function in LoopStartCutProcessor::UpdateCache and do not call this at other place.
	static Point2D CalcPiercePt(const LoopStartCutData* pLoopStartCutData);

	// get pierce/un-pierce pt for "line" type lead.
	static Point2D GetLineLeadPiercePt(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get pierce/un-pierce pt for "two line" type lead.
	// notes:
	//   1) we know that the do-pierce point of this lead is vertical to the cut direction, that means 
	//      the vector from the start point to do-pierce point is vertical to the cut direction vector.
	static Point2D GetTwoLineLeadPiercePt(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get pierce/un-pierce pt for "arc" type lead.
	static Point2D GetArcLeadPiercePt(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get pierce/un-pierce pt for "line arc" type lead.
	static Point2D GetLineArcLeadPiercePt(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get pierce/un-pierce pt for "hook" type lead.
	static Point2D GetHookLeadPiercePt(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);	
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
