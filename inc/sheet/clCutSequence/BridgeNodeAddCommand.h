#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)
DECLARE_CUTLEADER_CLASS(HostSeqMover)

BEGIN_CUTLEADER_NAMESPACE()

// �ڡ��Žӽڵ㡱�����һ������·ʵ������Ϊ�����Žӽڵ㡱��
// ע��
//  1) �����Žӵġ���·ʵ�����Ͽ������������򣬴�ʱ��Ҫɾ���Ǹ�����
class ClCutSequence_Export BridgeNodeAddCommand : public SequenceCommandBase
{
public:
	BridgeNodeAddCommand(const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData,
		PartInstanceListPtr pPartInstances, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pParentBdgeNode, BridgePtPtr pParentBridgePt,
		LoopInstancePtr pLoopInstance, BridgePtPtr pChildBdgePt);
	~BridgeNodeAddCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// ��ǰ�޸ĵġ��Žӹ��򡱡�
	BridgeSequencePtr m_pBdgeSeq;

	// ��������Žӽڵ㡱�Ϲ��µĽڵ㡣
	BridgeNodePtr m_pParentBdgeNode;
	BridgePtPtr m_pParentBridgePt;

	// �����������·ʵ�����½����Žӽڵ㡱��
	LoopInstancePtr m_pLoopInstance;
	BridgePtPtr m_pChildBdgePt;

	// �������·ʵ�������и��Ҫ��Ҫ������
	BOOL m_bReverseCutDir;

	// �����������
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;
};

END_CUTLEADER_NAMESPACE()
