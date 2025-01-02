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


// ���Žӹ��򡱹�������
class ClCutSequence_Export BridgeSequenceManager
{
public:
	/************************************************************************/
	// ���ڴ������Žӹ��򡱡�

	// ����2������·ʵ�����������Žӹ��򡱡�
	static BridgeSequencePtr BuildBdgeSeq(LoopInstancePtr pFstLoopInst, BridgePtPtr pFstBdgePt, LoopInstancePtr pSndLoopInst, BridgePtPtr pSndBdgePt, double dBridgeWidth);

	// ���ݡ���·ʵ�����ϵĵ����������Žӵ㡱��
	static BridgePtPtr BuildBdgePt(LoopInstancePtr pLoopInstance, int iLoopNodeIndex, const Point2D& bdgePt);

	// �ж�����2����ͼ��ʵ������ָ����λ���ܲ��ܽ����Žӡ�
	// ע��
	//   1) ���ŵĿ���㹻Сʱ�������ܽ����Žӵġ�
	//   2) ���ڽ����Ž�ʱ���Žӵ�λ�ÿ��ܱ�������
	static BOOL TwoLoopCanBdge(LoopInstancePtr pFstLoopInst, int& iFstLoopNodeIndex, Point2D& fstBdgePt, LoopInstancePtr pSndLoopInst, int& iSndLoopNodeIndex, Point2D& sndBdgePt,
							   double dBdgeWidth);

	// �����2����ͼ��ʵ�������ܲ����Žӡ�
	// ע��
	//  1) ����2������·ʵ�����϶��С��Žӹ��򡱣�����ͬһ�����Žӹ���ʱ�������Ž����ǡ�
	static BOOL TwoLoopCanBdge(CutSequenceListPtr pCutSequences, const LoopInstance* pFstLoopInst, const LoopInstance* pSndLoopInst);
	/************************************************************************/


	/************************************************************************/
	// ���ڵõ��ŽӵĻ���ͼ�Σ��������ϵ�£���

	// �õ����Žӽڵ㡱��ĳһ���������Žӵ㡱���ڵġ�����ͼ�Ρ����������ϵ�£���
	// ע��
	//   1) ������ͼ�Ρ��ķ�����ѭ�и��
	static ILineArc2DPtr GetOutBdgePtGeom(BridgeNodePtr pBridgeNode, int iBdgePtIdx);
	static ILineArc2DPtr GetOutBdgePtGeom(BridgeNodePtr pBridgeNode, BridgePtPtr pBdgePt);

	// �õ����Žӽڵ㡱�ġ�����Žӵ㡱���ڵġ�����ͼ�Ρ����������ϵ�£���
	// ע��
	//   1) ������ͼ�Ρ��ķ�����ѭ�и��
	static ILineArc2DPtr GetInBdgePtGeom(BridgeNodePtr pBridgeNode);

	// �õ����С��Žӽڵ㡱�����ġ�����š��Ļ���ͼ�Ρ�
	// ע��
	//  1) �����ڵ㡱�����ء���Ϊ��û�С�����š���
	static std::vector<std::pair<BridgeNodePtr, LineArc2DListPtr> > GetAllBdgeGeomInfo(CutSequenceListPtr pCutSequences);

	// ��������׽ڵ����µݹ飬�õ�ÿ���ŵĻ���ͼ�Ρ�
	// ע��
	//   1) �������������������ʱ����ע�⡰pBridgeNode��������Ҷ�ӽڵ㡣
	//   2) ���Ǹ��ݹ麯����
	static void GetBdgeGeomInfo(BridgeNodePtr pBridgeNode, std::vector<std::pair<BridgeNodePtr, LineArc2DListPtr>>& bdgeGeomInfo);

	// �õ���Ҷ���Žӽڵ㡱�ġ�����š��Ļ���ͼ�Ρ�
	static LineArc2DListPtr GetBdgeGeom(BridgeNodePtr pSubBdgeNode);
	/************************************************************************/


	/************************************************************************/
	// ���ڵõ����Žӵ㡱���ŵ㡱���ꡣ

	// �õ����Žӵ㡱���ꡣ
	static Point2D GetBdgePosition(LoopInstancePtr pLoopInstance, BridgePtPtr pBridgePt);

	// �õ����Žӽڵ㡱��ĳ���������Žӵ㡱��2�����ŵ㡱���ꡣ
	/* ������
	*  iBdgePtIdx: �������Žӵ㡱������
	*/
	// ע��
	//   1) ��2�����ŵ㡱����ʵ���и�˳������
	static Point2DListPtr GetTwoOutBdgePt(BridgeNodePtr pBridgeNode, int iBdgePtIdx);

	// �õ����Žӽڵ㡱�ġ�����Žӵ㡱��2�����ŵ㡱���ꡣ
	/* ������
	*  parentOutBdgePt: �������Žӽڵ㡱��Ӧ�ġ������Žӵ㡱���ꡣ
	*/
	// ע��
	//   1) ��2�����ŵ㡱����ʵ���и�˳������
	static Point2DListPtr GetTwoInBdgePt(Point2D parentOutBdgePt, BridgeNodePtr pBridgeNode);
	/************************************************************************/


	// ������ڵ�ġ����ڵ㡱��Ϊ�Լ��ġ��ӽڵ㡱��
	// ע��
	//  1) ���������Ҫ�ݹ鵽���ڵ㣬��������ڵ�ͱ���˸��ڵ㡣
	static void SetAsParent(BridgeNodePtr pBridgeNode);

	// ��������Žӽڵ㡱���������ڵ㡱��
	static BridgeNodePtr GetRootBdgeNode(BridgeNodePtr pBridgeNode);

	// ƫ�ơ��Žӹ��򡱡�
	// ע��
	//   1) ��������á�����ƫ��֮�󣬡��Žӹ���Ҳ��Ҫ��ƫ�ơ�
	static BridgeSequencePtr OffsetCutSequence(BridgeSequencePtr pBridgeSeq, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);

	// �����µġ����ʵ�������ϸ��¡��Žӹ����е�һЩ���ݡ�
	/* ������
	*  pOldPartInsts: �ɵ����ʵ�����ϡ�
	*  pNewPartInsts: ��¡���µ����ʵ�����ϡ�
	*/
	// ע��
	//  1) ��Ҫ�滻������·ʵ�����еĻ�·ID�͡����ʵ������
	//  2) ��Ҫ�滻�����Žӵ㡱�е�ͼ��ID��
	//  3) ���Ǹ��ݹ麯����
	static void UpdateCutSequence(BridgeNodePtr pBridgeNode, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);

	// �ж�����Žӹ����ǲ�����Ч��
	// ע��
	//  1) ��Ҫ��⡰�Žӹ����漰�ġ���·ʵ�������ڲ��ڡ�
	//  2) ��Ҫ��⡰�Žӹ����漰�ġ���·ʵ�������и���ǲ���һ���ġ�
	static BOOL IsCutSequenceInvalid(BridgeSequencePtr pBridgeSeq, PartInstanceListPtr pAllPartInst);

private:
	// �滻�����С��Žӽڵ㡱�С���·ʵ���������еġ����ʵ��������
	// ע��
	//  1) ���Ǹ��ݹ麯����
	static void ReplacePartInst(BridgeNodePtr pBridgeNode, PartInstanceListPtr pPartInstances);

	// �����������ǰ�����һ�����߰���֮��Ĵ��ŷ�ʽ��
	/* ������
	*  pCornerDock1, pCornerDock2: �������ǰ�����
	*  pEdgeDock: ���߰�����
	*  dockPt: ���߰���������Žӵ㡣
	*  bCCW: Ϊ�棬�ŵĵ�һ����ͨ�����ο��ߡ���ʱ����ת�õ�������˳ʱ����ת�õ���
	*  retPt: ���ص�ĳһ�����ǰ����ϵ��Žӵ㡣
	*/
	// ע��
	//  1) �����������һ�����ŷ������ŵĵ�һ���ߵ�ȷ�������ַ�ʽ��ÿ�ַ�ʽȷ��һ�����ŷ�����
	static BOOL GetCornerBdgePt(ILineArc2DPtr pCornerDock1, ILineArc2DPtr pCornerDock2, const Point2D& cornerPt, ILineArc2DPtr pEdgeDock, const Point2D& dockPt,
		double dBdgeWid, BOOL bCCW,	Point2D& retPt);

	// �����������ǰ����������������ǰ���֮��Ĵ��ŷ�ʽ��
	/* ������
	*/
	// ע��
	//  1) �������ǰ���֮��ļ��ŷ�ʽ�����֣�
	//     a) �����ǵ��������ļ��ŷ�ʽ��
	//     b) ���ļ��ŷ�ʽ��
	static BOOL GetCornerBdgePt(ILineArc2DPtr pCornerDock1, const Point2D& cornerPt1, ILineArc2DPtr pCornerDock2, const Point2D& cornerPt2, double dBdgeWid, Point2D& retPt1, Point2D& retPt2);
};

END_CUTLEADER_NAMESPACE()
