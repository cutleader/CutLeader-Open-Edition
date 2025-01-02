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

// “桥接节点”。
// 注：
//   1) 可以把一个“桥接节点”看成一个“零件实例”。
//   2) “桥接节点”结构像一棵树一样，根节点是开始切割节点。
//   3) 一个“桥接节点”有一个入口“桥接点”，入口“桥接点”连着父亲“桥接节点”；可以有多个出口“桥接点”，出口“桥接点”连着孩子“桥接节点”。
//   4) 待完善：这个类保持了父节点的智能指针，存在互相引用的情况，释放时导致内存泄漏。
class ClCutSequence_Export BridgeNode : public ClData
{
public:
	BridgeNode(void);
	BridgeNode(LoopInstancePtr pLoopInstance);
	virtual ~BridgeNode(void);

public: // implement IData interface.
	// 克隆“桥接节点”。
	// 注：
	//  1) 递归克隆子节点。
	//  2) 不克隆“连接键”。
	//  3) 克隆“回路实例”。
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
	// 得到这个“桥接节点”中所有的“回路实例/零件实例”。
	// 注：
	//   1) 这是个递归函数。
	void GetAllLoopInsts(LoopInstanceListPtr pLoopInstList);
	void GetAllPartInsts(PartInstanceListPtr pPartInstList);

	// 得到这个“桥接节点”的所有“出口桥接点”。
	BridgePtListPtr GetOutBridgePts();

	// 得到“回路实例”对应的“子桥接节点”。
	// 注：
	//   1) 这是个递归函数。
	BridgeNodePtr GetSubNodeByLoopInst(LoopInstancePtr pLoopInstance);

	// 这个“桥接节点”的子节点中是否包含“pBridgeNode”。
	BOOL IncludeBdgeNode(BridgeNodePtr pBridgeNode);

	// 重置父亲节点。
	void ResetParentNode() { m_pParentBdgeNode.reset(); }

	// 重置“连接键”。
	void ResetBdgeLink() { m_pBridgeLink.reset(); }

protected:
	// “桥接节点”对应的零件的外围回路。
	LoopInstancePtr m_pLoopInstance;

	// 父亲“桥接节点”。
	// 注：
	//   1) 当这个“桥接节点”是起始节点时，这个变量为空。
	BridgeNodePtr m_pParentBdgeNode;

	// 该“桥接节点”与父亲节点的“连接键”。
	// 注：
	//   1) 当这个“桥接节点”是起始节点时，这个变量为空。
	BridgeLinkPtr m_pBridgeLink;

	// 子“桥接节点”。
	// 注：
	//   1) 当这个“桥接节点”是最下层时，这个变量中元素个数为零。
	BridgeNodeListPtr m_pSubBridgeNodes;
};

END_CUTLEADER_NAMESPACE()
