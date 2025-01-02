#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(ICutPartSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ��2������·ʵ�����������Žӹ��򡱵����
// ע��
//  1) ��2������·ʵ�����Ͽ϶�û�С��Žӹ��򡱣����ǿ���������������ʱ��Ҫɾ����Щ���еĹ���
class ClCutSequence_Export BridgeSequenceAddCommand : public SequenceCommandBase
{
public:
	BridgeSequenceAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartInstanceListPtr pPartInstances,
				  LoopInstancePtr pFstLoopInst, BridgePtPtr pFstBdgePt, LoopInstancePtr pSndLoopInst, BridgePtPtr pSndBdgePt);
	virtual ~BridgeSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// �����������Žӹ��򡱵ĵ�һ����·��Ϣ��
	LoopInstancePtr m_pFstLoopInst;
	BridgePtPtr m_pFstBdgePt;

	// �����������Žӹ��򡱵ĵڶ�����·��Ϣ��
	LoopInstancePtr m_pSndLoopInst;
	BridgePtPtr m_pSndBdgePt;

	// �·�������ʵ���ڲ������Ĺ���
	CutSequenceListPtr m_pInnerLoopSeqs;
	LONGLONG m_iNewNextSeqID; // ��Щ����ĺ�һ������-1�Ļ�������������

	// �ڶ�������·ʵ�������и��Ҫ��Ҫ������
	BOOL m_bReverseCutDir;

	// ��2������·ʵ�����Ͽ������еĹ���
	// ע��
	//  1) ע��������λ�õ�������ϵ��
	ICutPartSequencePtr m_pFstExistSeq;
	LONGLONG m_iFstNextSeqID;
	ICutPartSequencePtr m_pSndExistSeq;
	LONGLONG m_iSndNextSeqID;

	// �½��ġ��Žӹ���
	BridgeSequencePtr m_pNewBdgeSeqUnit;
	LONGLONG m_iBdgeNextSeqID; // �Žӹ�������������ǰ�棬-1�Ͳ嵽���
};

END_CUTLEADER_NAMESPACE()
