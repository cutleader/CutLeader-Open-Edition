#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)

BEGIN_CUTLEADER_NAMESPACE()

// �޸ġ��Žӹ��򡱵���ʼ����·ʵ������
class ClCutSequence_Export StartBridgeNodeChangeCommand : public SequenceCommandBase
{
public:
	StartBridgeNodeChangeCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode);
	virtual ~StartBridgeNodeChangeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵġ��Žӹ��򡱡�
	BridgeSequencePtr m_pBridgeSequence;

	// ���������·ʵ������Ϊ��ʼ�ġ�
	BridgeNodePtr m_pBridgeNode;

	/************************************************************************/
	// ��������ʱ���ܻ��õ������ݡ�

	// ���Žӹ�����ԭ���򼯺��е�λ�á�
	int m_iExistSeqIndex;

	// �ϵ���ʼ�ڵ㡣
	BridgeNodePtr m_pOldStartNode;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
