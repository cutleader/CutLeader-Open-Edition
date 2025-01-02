#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(ICutPartSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 从2个“回路实例”创建“桥接工序”的命令。
// 注：
//  1) 这2个“回路实例”上肯定没有“桥接工序”，但是可能有其他工序，这时需要删除那些已有的工序。
class ClCutSequence_Export BridgeSequenceAddCommand : public SequenceCommandBase
{
public:
	BridgeSequenceAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartInstanceListPtr pPartInstances,
				  LoopInstancePtr pFstLoopInst, BridgePtPtr pFstBdgePt, LoopInstancePtr pSndLoopInst, BridgePtPtr pSndBdgePt);
	virtual ~BridgeSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 用来创建“桥接工序”的第一个回路信息。
	LoopInstancePtr m_pFstLoopInst;
	BridgePtPtr m_pFstBdgePt;

	// 用来创建“桥接工序”的第二个回路信息。
	LoopInstancePtr m_pSndLoopInst;
	BridgePtPtr m_pSndBdgePt;

	// 新分配的零件实例内部轮廓的工序。
	CutSequenceListPtr m_pInnerLoopSeqs;
	LONGLONG m_iNewNextSeqID; // 这些工序的后一个工序，-1的话这个工序在最后。

	// 第二个“回路实例”的切割方向要不要调整。
	BOOL m_bReverseCutDir;

	// 这2个“回路实例”上可能已有的工序。
	// 注：
	//  1) 注意这两个位置的依赖关系。
	ICutPartSequencePtr m_pFstExistSeq;
	LONGLONG m_iFstNextSeqID;
	ICutPartSequencePtr m_pSndExistSeq;
	LONGLONG m_iSndNextSeqID;

	// 新建的“桥接工序”
	BridgeSequencePtr m_pNewBdgeSeqUnit;
	LONGLONG m_iBdgeNextSeqID; // 桥接工序插在这个工序前面，-1就插到最后。
};

END_CUTLEADER_NAMESPACE()
