#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeNodeList)
DECLARE_CUTLEADER_CLASS(BridgeLink)
DECLARE_CUTLEADER_CLASS(BridgePtList)

BEGIN_CUTLEADER_NAMESPACE()

// ���Žӽڵ㡱��
// ע��
//   1) ���԰�һ�����Žӽڵ㡱����һ�������ʵ������
//   2) ���Žӽڵ㡱�ṹ��һ����һ�������ڵ��ǿ�ʼ�и�ڵ㡣
//   3) һ�����Žӽڵ㡱��һ����ڡ��Žӵ㡱����ڡ��Žӵ㡱���Ÿ��ס��Žӽڵ㡱�������ж�����ڡ��Žӵ㡱�����ڡ��Žӵ㡱���ź��ӡ��Žӽڵ㡱��
//   4) �����ƣ�����ౣ���˸��ڵ������ָ�룬���ڻ������õ�������ͷ�ʱ�����ڴ�й©��
class ClCutSequence_Export BridgeNode : public ClData
{
public:
	BridgeNode(void);
	BridgeNode(LoopInstancePtr pLoopInstance);
	virtual ~BridgeNode(void);

public: // implement IData interface.
	// ��¡���Žӽڵ㡱��
	// ע��
	//  1) �ݹ��¡�ӽڵ㡣
	//  2) ����¡�����Ӽ�����
	//  3) ��¡����·ʵ������
	virtual IDataPtr Clone() const override;

public: // set/get functions.
	void SetLoopInst(LoopInstancePtr pLoopInstance) { m_pLoopInstance = pLoopInstance; }
	LoopInstancePtr GetLoopInstance() const { return m_pLoopInstance; }

	void SetParentBdgeNode(BridgeNodePtr pParentBdgeNode) { m_pParentBdgeNode = pParentBdgeNode; }
	BridgeNodePtr GetParentBdgeNode() const { return m_pParentBdgeNode; }

	void SetBridgeLink(BridgeLinkPtr pBridgeLink) { m_pBridgeLink = pBridgeLink; }
	BridgeLinkPtr GetBridgeLink() const { return m_pBridgeLink; }

	void SetSubBridgeNodes(BridgeNodeListPtr pSubBridgeNodes) { m_pSubBridgeNodes = pSubBridgeNodes; }
	BridgeNodeListPtr GetSubBridgeNodes() const { return m_pSubBridgeNodes; }

public:
	// �õ�������Žӽڵ㡱�����еġ���·ʵ��/���ʵ������
	// ע��
	//   1) ���Ǹ��ݹ麯����
	void GetAllLoopInsts(LoopInstanceListPtr pLoopInstList);
	void GetAllPartInsts(PartInstanceListPtr pPartInstList);

	// �õ�������Žӽڵ㡱�����С������Žӵ㡱��
	BridgePtListPtr GetOutBridgePts();

	// �õ�����·ʵ������Ӧ�ġ����Žӽڵ㡱��
	// ע��
	//   1) ���Ǹ��ݹ麯����
	BridgeNodePtr GetSubNodeByLoopInst(LoopInstancePtr pLoopInstance);

	// ������Žӽڵ㡱���ӽڵ����Ƿ������pBridgeNode����
	BOOL IncludeBdgeNode(BridgeNodePtr pBridgeNode);

	// ���ø��׽ڵ㡣
	void ResetParentNode() { m_pParentBdgeNode.reset(); }

	// ���á����Ӽ�����
	void ResetBdgeLink() { m_pBridgeLink.reset(); }

protected:
	// ���Žӽڵ㡱��Ӧ���������Χ��·��
	LoopInstancePtr m_pLoopInstance;

	// ���ס��Žӽڵ㡱��
	// ע��
	//   1) ��������Žӽڵ㡱����ʼ�ڵ�ʱ���������Ϊ�ա�
	BridgeNodePtr m_pParentBdgeNode;

	// �á��Žӽڵ㡱�븸�׽ڵ�ġ����Ӽ�����
	// ע��
	//   1) ��������Žӽڵ㡱����ʼ�ڵ�ʱ���������Ϊ�ա�
	BridgeLinkPtr m_pBridgeLink;

	// �ӡ��Žӽڵ㡱��
	// ע��
	//   1) ��������Žӽڵ㡱�����²�ʱ�����������Ԫ�ظ���Ϊ�㡣
	BridgeNodeListPtr m_pSubBridgeNodes;
};

END_CUTLEADER_NAMESPACE()
