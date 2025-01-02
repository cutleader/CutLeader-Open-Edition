#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// ����·ʵ������������
class ClPartLayout_Export LoopInstanceManager
{
public:
	/************************************************************************/
	// ���ڡ���·ʵ�����������г��㡣

	// �õ�����·ʵ����������㡣
	static Point2D GetPiercePt(LoopInstancePtr pLoopInstance);

	// �õ�����·ʵ�������г��㡣
	/* params:
	*  bArrayLastInst: whether the un-pierce point will on the last inst.
	*/
	static Point2D GetUnPiercePt(LoopInstancePtr pLoopInstance, BOOL bArrayLastInst = TRUE);
	/************************************************************************/


	/************************************************************************/
	// ���ڵõ���·ʵ����

	// �����ʵ���õ���·ʵ����
	static LoopInstanceListPtr GetLoopInstanceList(PartInstanceListPtr pPartInstances);
	static LoopInstanceListPtr GetLoopInstanceList(PartInstancePtr pPartInstance);

    // �õ�����ʵ������Щ����ʵ��û�б�����Ϊ�����и��
    static LoopInstanceListPtr GetLoopInstances_hasValidTool(PartInstancePtr pPartInstance);

	// �õ����ʵ������Χ����·ʵ������
	// ע��
	//  1) ������û����Χ��·�Ļ��������ء�
	static LoopInstanceListPtr GetOuterLoopInsts(PartInstanceListPtr pPartInstances);
	static LoopInstancePtr GetOuterLoopInst(PartInstancePtr pPartInstance);

	// �õ����ʵ�����ڲ�����·ʵ������
	static LoopInstanceListPtr GetInnerLoopInsts(PartInstancePtr pPartInstance);

	// �õ����������ġ���·ʵ������
	static LoopInstancePtr GetClosestLoopInst(LoopInstanceListPtr pLoopInstList, const Point2D& pt);
    // �õ����������ġ���·ʵ�������������ʵ��û�б�����Ϊ�����и��
    static LoopInstancePtr GetClosestLoopInst_hasValidTool(LoopInstanceListPtr pLoopInstList, const Point2D& pt);

	// �õ����������Ļ�·��
	// ע��
	//  1) ֻ����Χ���ΰ����㡰pt���ġ���·ʵ�������ܱ���Ϊ��ѡ�ģ�ע������ġ���Χ���Ρ���У�����������Ρ�
	static LoopInstancePtr GetClosestLoopInstEx(LoopInstanceListPtr pLoopInstList, const Point2D& pt);

	// �õ����ο��ڵĻ�·ʵ����
	static LoopInstanceListPtr GetLoopInstsInRect(LoopInstanceListPtr pLoopInstList, const Rect2D& rect);
	/************************************************************************/


	/************************************************************************/
	// ���ڱպϡ���·ʵ�������и��

	// �õ��պϡ���·ʵ�������и��
	static DIRECTION GetCloseLoopInstDir(LoopInstancePtr pLoopInstance);

	// ��Щ�պϡ���·ʵ�������и���ǲ���һ����
	static BOOL CloseLoopSameDir(LoopInstanceListPtr pLoopInstList);
	/************************************************************************/

	// ���»�·ʵ���Ļ��档
	static void UpdateCache(LoopInstancePtr pLoopInstance);
	static void UpdateCache(LoopInstancePtr pLoopInstance, LoopCutFeaturePtr pLoopFeature);

	// �ѡ�����ʵ����ת��Ϊ����Ρ�
	// ע��
	//  1) ֻ��ת��յ�����������array loop��ֻ������һ����Ҳ��֪������һ����
	static Polygon2DPtr LoopInst2Poly(LoopInstancePtr pLoopInstance);

	// ����LineArc2DLoop����������յ�loop��
	static LineArc2DLoopListPtr LoopInsts_2_GeomLoops(const LoopInstanceList* pLoopInstances);
	static LineArc2DLoopPtr LoopInst_2_GeomLoop(const LoopInstance* pLoopInstance);

	// �õ����������ͺ��LineArc2DLoop���贫���յ���������
	static LineArc2DLoopPtr ExpandLoopInst(LoopInstancePtr pLoopInstance, double dExpandValue);

	// ���ǲ�����ĳ������ڲ���
	// ע��pLoopInsts_boundary�����ʵ������������
	static bool Point_is_insidePartInst(const Point2D& pt, LoopInstanceListPtr pLoopInsts_boundary);
};

END_CUTLEADER_NAMESPACE()
