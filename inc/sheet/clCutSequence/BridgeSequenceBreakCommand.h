#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgeLink)

BEGIN_CUTLEADER_NAMESPACE()

// 在“入口桥接点”处断开“桥接节点”。
// 注：
//  1) 我们把打断后的2段节点叫做“前一段节点”和“后一段节点”。
class ClCutSequence_Export BridgeSequenceBreakCommand : public SequenceCommandBase
{
	// 定义了四种打断“桥接工序”方式。
	typedef enum tagBreakBdgeType
	{
		BREAK_BDGE_1	= 1, // “前一段节点”已不能成为“桥接工序”，“后一段节点”可以成为新的“桥接工序”。
		BREAK_BDGE_2	= 2, // “前一段节点”和“后一段节点”都不能成为“桥接工序”。
		BREAK_BDGE_3	= 3, // “前一段节点”组成的工序保留，“后一段节点”不能成为“桥接工序”。
		BREAK_BDGE_4	= 4, // “前一段节点”组成的工序保留，“后一段节点”可以成为新的“桥接工序”。
	} BREAK_BDGE_TYPE;

public:
	BridgeSequenceBreakCommand(const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode);
	~BridgeSequenceBreakCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// 打断“桥接工序”方式。
	BREAK_BDGE_TYPE m_iBreakType;

	// 要断开的“桥接工序”。
	BridgeSequencePtr m_pBridgeSequence;

	// 在这个“桥接节点”的“入口桥接点”处断开。
	BridgeNodePtr m_pBridgeNode;

	/************************************************************************/
	// 回退命令时可能会用到的数据。

	// 断开的“连接键”的父亲端节点。
	BridgeNodePtr m_pParentBdgeNode;

	// 断开的“连接键”。
	BridgeLinkPtr m_pBdgeLink;

	// 要断开的“桥接工序”在原工序集合中的位置。
	int m_iExistSeqIndex;

	// 根据“后一段节点”新建的工序，可能为空。
	BridgeSequencePtr m_pNewBdgeSeqUnit;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
