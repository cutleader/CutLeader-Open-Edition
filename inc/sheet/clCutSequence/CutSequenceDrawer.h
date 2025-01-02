#pragma once

#include "clCutSequenceCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// ���������������
// ע��
//   1) ���ƹ�����ʵ���ǻ����и�·�����漰���������ֻ���ʱ����ʹ�ù�����ɫ�����Ǳ�������ԭ����ɫ��
//   2) ����ʱ������ƫ�ƣ���Ϊ��Ͼ������и�ģ�⡣
class ClCutSequence_Export CutSequenceDrawer : public BasicGeometryDrawer
{
public:
	CutSequenceDrawer(GlViewPortPtr pViewPort);
	~CutSequenceDrawer(void);

public:
	// ���Ƶ�ǰ�Ĺ���
	/* ������
	*  pCutNodeList: �����и�ڵ㡣
	*/
	void DrawActiveCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawActiveCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// ����ѡ�еĹ���
	/* ������
	*  pCutNodeList: �����и�ڵ㡣
	*/
	void DrawSelectedCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawSelectedCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// ���ơ�ģ����״����
	// ע��
	//  1) �����ƶ��ߵ���������������ж�Ϊ���ߣ�������ͼ��ʽ�ɵ����߸������á�
    //  2) bIgnoreCutPathOnLoop: �Ƿ񲻻������ϵ��и�ڵ㣬�������Ա���������ϵĵ�����ɫ���ǵ���
	void DrawSequenceSimulateShapes(const SequenceSimulateShapeList* pSequenceSimulateShapes, bool bIgnoreCutPathOnLoop = true);

	// ���ƹ���
	// ע��
	//  1) �����и����ͼ��ʽ�����á�
	void DrawCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// ���ơ�����ڵ㡱��
	// ע��
	//  1) ������㡱�ڵ���ʮ�ֲ��ʾ�����ڡ������·���ڵ�ͻ���������
    //  2) ��Щ�����������û�ͼ��ʽ�����۸�������ѡ�У����ı�������ɫ�������������͵�����ڵ����ϵͳ�ĸ�����ѡ��ɫ��
    void DrawActiveChainNode(const IChainNode* pChainNode);
    void DrawSelectedChainNode(const IChainNode* pChainNode);

private:
    // ����һЩģ��ͼ�ε�������Ƽ�ͷ��
    // ע��
    //  1��һ�㻭�ڿ����ƶ��ߣ������и��ߣ��Žӹ����е��ŵ����࣬�ȵȡ�
    void DrawArrow(const ILineArc2D* pLineArc2D);
};

END_CUTLEADER_NAMESPACE()
