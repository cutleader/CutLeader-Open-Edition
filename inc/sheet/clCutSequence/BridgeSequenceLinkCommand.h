#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)

BEGIN_CUTLEADER_NAMESPACE()

// 链接2个“桥接工序”。
// 注：
//  1) 把第二个“桥接工序”的某个“桥接节点”挂到第一个“桥接工序”的某个“桥接节点”下。
//  2) 第二个“桥接工序”的某个“桥接节点”的父子关系需要重新整理。
//  3) 删除第二个“桥接工序”。
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
	// 第一个“桥接工序”相关信息。
	BridgeSequencePtr m_pFstBdgeSeq;
	BridgeNodePtr m_pFstBdgeNode;
	BridgePtPtr m_pFstBdgePt;

	// 第二个“桥接工序”相关信息。
	BridgeSequencePtr m_pSndBdgeSeq;
	BridgeNodePtr m_pSndBdgeNode;
	BridgePtPtr m_pSndBdgePt;

	// 第二个“桥接工序”的切割方向要不要调整。
	BOOL m_bReverseCutDir;

	// 第二个“桥接工序”在原有“工序集合”中的位置。
	int m_iSndSeqIndex;

	// 第二个“桥接工序”原有的根节点，回退命令时用到。
	BridgeNodePtr m_pRootBdgeNode;
};

END_CUTLEADER_NAMESPACE()
