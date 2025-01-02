#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(ChainNodeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export ChainNodeList : public std::vector<IChainNodePtr>
{
public:
	ChainNodeList(void);
	~ChainNodeList(void);

public:
	// ����ڵ��Ƿ���ڡ�
	BOOL IsNodeExist(LONGLONG iNodeID) const;

	// �õ��ڵ�������
	int GetNodeIndex(IChainNodePtr pChainNode) const;

	// ɾ��������ڵ㡱��
	void DelChainNode(LONGLONG iNodeID);

	// �ڡ�iNextNodeID��ǰ����һ���ڵ㡣
	// ע��
	//  1) -1���嵽���
	void InsertNode(LONGLONG iNextNodeID, IChainNodePtr pChainNode);

	// ������ڵ�ǰ�����һЩ����ڵ�
	void InsertChainNode_before_chainNode(IChainNodePtr pChainNode, ChainNodeListPtr pChainNodeList);

	// �õ����������Ľڵ㡣
	IChainNodePtr GetClosestNode(const Point2D& pt) const;

	// �õ��ýڵ����һ���ڵ㡣
	// ע��
	//  1) ����ýڵ��Ѿ������һ��������-1��
	LONGLONG GetNextNodeID(IChainNodePtr pChainNode) const;
};

END_CUTLEADER_NAMESPACE()
