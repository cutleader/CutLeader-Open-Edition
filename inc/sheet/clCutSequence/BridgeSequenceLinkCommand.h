#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)

BEGIN_CUTLEADER_NAMESPACE()

// ����2�����Žӹ��򡱡�
// ע��
//  1) �ѵڶ������Žӹ��򡱵�ĳ�����Žӽڵ㡱�ҵ���һ�����Žӹ��򡱵�ĳ�����Žӽڵ㡱�¡�
//  2) �ڶ������Žӹ��򡱵�ĳ�����Žӽڵ㡱�ĸ��ӹ�ϵ��Ҫ��������
//  3) ɾ���ڶ������Žӹ��򡱡�
class ClCutSequence_Export BridgeSequenceLinkCommand : public SequenceCommandBase
{
public:
	BridgeSequenceLinkCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, BridgeSequencePtr pFstBdgeSeq, BridgeNodePtr pFstBdgeNode, BridgePtPtr pFstBdgePt,
		BridgeSequencePtr pSndBdgeSeq, BridgeNodePtr pSndBdgeNode, BridgePtPtr pSndBdgePt);
	~BridgeSequenceLinkCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ��һ�����Žӹ��������Ϣ��
	BridgeSequencePtr m_pFstBdgeSeq;
	BridgeNodePtr m_pFstBdgeNode;
	BridgePtPtr m_pFstBdgePt;

	// �ڶ������Žӹ��������Ϣ��
	BridgeSequencePtr m_pSndBdgeSeq;
	BridgeNodePtr m_pSndBdgeNode;
	BridgePtPtr m_pSndBdgePt;

	// �ڶ������Žӹ��򡱵��и��Ҫ��Ҫ������
	BOOL m_bReverseCutDir;

	// �ڶ������Žӹ�����ԭ�С����򼯺ϡ��е�λ�á�
	int m_iSndSeqIndex;

	// �ڶ������Žӹ���ԭ�еĸ��ڵ㣬��������ʱ�õ���
	BridgeNodePtr m_pRootBdgeNode;
};

END_CUTLEADER_NAMESPACE()
