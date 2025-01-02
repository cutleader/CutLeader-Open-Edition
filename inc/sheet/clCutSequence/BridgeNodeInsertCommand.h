#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)
DECLARE_CUTLEADER_CLASS(HostSeqMover)

BEGIN_CUTLEADER_NAMESPACE()

// �ڡ��Žӽڵ㡱ǰ���һ������·ʵ������Ϊ�������Žӽڵ㡱��
// ע��
//  1) �����Žӵġ���·ʵ�����Ͽ������������򣬴�ʱ��Ҫɾ���Ǹ�����
//  2) �²���ġ��Žӽڵ㡱����ˡ����ڵ㡱��
class ClCutSequence_Export BridgeNodeInsertCommand : public SequenceCommandBase
{
public:
	BridgeNodeInsertCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData,
					  PartInstanceListPtr pPartInstances, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode, BridgePtPtr pBridgePt, LoopInstancePtr pLoopInstance, BridgePtPtr pParentBdgePt);
	~BridgeNodeInsertCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ��ǰ�޸ĵġ��Žӹ��򡱡�
	BridgeSequencePtr m_pBdgeSeq;

	// ��������Žӽڵ㡱ǰ�����µĽڵ㡣
	BridgeNodePtr m_pBridgeNode;
	BridgePtPtr m_pBridgePt;

	// �����������·ʵ�����½����Žӽڵ㡱��
	LoopInstancePtr m_pLoopInstance;
	BridgePtPtr m_pParentBdgePt;

	// ���С��Žӹ��򡱵��и��Ҫ��Ҫ������
	BOOL m_bReverseCutDir;

	// ԭ�еĸ��ڵ㣬��������ʱ�õ���
	BridgeNodePtr m_pRootBdgeNode;

	// �����������
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;
};

END_CUTLEADER_NAMESPACE()
