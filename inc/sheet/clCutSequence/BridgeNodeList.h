#pragma once

#include "clCutSequenceCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(BridgeNode)

BEGIN_CUTLEADER_NAMESPACE()

// ���Žӽڵ㡱���ϡ�
class BridgeNodeList : public std::vector<BridgeNodePtr>
{
public:
	BridgeNodeList(void);
	~BridgeNodeList(void);

public:
	// ���ݡ���·ʵ����ɾ�����Žӽڵ㡱��
	// ע��
	//   1) �ⲻ�ǵݹ麯����
	void DelBdgeNodeByLoop(LoopInstancePtr pLoopInstance);

	// ɾ��ָ���ġ��Žӽڵ㡱��
	// ע��
	//   1) �ⲻ�ǵݹ麯����
	void DelBdgeNodeByID(LONGLONG iBdgeNodeID);
};

END_CUTLEADER_NAMESPACE()
