#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeLink)

BEGIN_CUTLEADER_NAMESPACE()

// �ڡ�����Žӵ㡱���Ͽ����Žӽڵ㡱��
// ע��
//  1) ���ǰѴ�Ϻ��2�νڵ������ǰһ�νڵ㡱�͡���һ�νڵ㡱��
class ClCutSequence_Export BridgeSequenceBreakCommand : public SequenceCommandBase
{
	// ���������ִ�ϡ��Žӹ��򡱷�ʽ��
	typedef enum tagBreakBdgeType
	{
		BREAK_BDGE_1	= 1, // ��ǰһ�νڵ㡱�Ѳ��ܳ�Ϊ���Žӹ��򡱣�����һ�νڵ㡱���Գ�Ϊ�µġ��Žӹ��򡱡�
		BREAK_BDGE_2	= 2, // ��ǰһ�νڵ㡱�͡���һ�νڵ㡱�����ܳ�Ϊ���Žӹ��򡱡�
		BREAK_BDGE_3	= 3, // ��ǰһ�νڵ㡱��ɵĹ�����������һ�νڵ㡱���ܳ�Ϊ���Žӹ��򡱡�
		BREAK_BDGE_4	= 4, // ��ǰһ�νڵ㡱��ɵĹ�����������һ�νڵ㡱���Գ�Ϊ�µġ��Žӹ��򡱡�
	} BREAK_BDGE_TYPE;

public:
	BridgeSequenceBreakCommand(const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode);
	~BridgeSequenceBreakCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// ��ϡ��Žӹ��򡱷�ʽ��
	BREAK_BDGE_TYPE m_iBreakType;

	// Ҫ�Ͽ��ġ��Žӹ��򡱡�
	BridgeSequencePtr m_pBridgeSequence;

	// ��������Žӽڵ㡱�ġ�����Žӵ㡱���Ͽ���
	BridgeNodePtr m_pBridgeNode;

	/************************************************************************/
	// ��������ʱ���ܻ��õ������ݡ�

	// �Ͽ��ġ����Ӽ����ĸ��׶˽ڵ㡣
	BridgeNodePtr m_pParentBdgeNode;

	// �Ͽ��ġ����Ӽ�����
	BridgeLinkPtr m_pBdgeLink;

	// Ҫ�Ͽ��ġ��Žӹ�����ԭ���򼯺��е�λ�á�
	int m_iExistSeqIndex;

	// ���ݡ���һ�νڵ㡱�½��Ĺ��򣬿���Ϊ�ա�
	BridgeSequencePtr m_pNewBdgeSeqUnit;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
