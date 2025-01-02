#pragma once

#include "SequenceCommandBase.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 创建“飞行切割工序”的命令。
// 注：
// 1) 如果这些轮廓上已经有了工序，那么删除这些工序，并把飞切工序插在之前的工序位置。如果这些轮廓上还没有工序，那么把飞切工序放在工序集合的最后。
class ClCutSequence_Export FlyCutSequenceAddCommand : public SequenceCommandBase
{
	// "替换信息"
	struct SequenceReplaceInfo
	{
		ICutSequencePtr m_pNewFlyCutSeq; // 新的飞行切割工序
		std::map<int, ICutSequencePtr> m_oldCutSequences; // 旧的工序，集合中可能为空。
	};

public:
	FlyCutSequenceAddCommand(const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartInstanceListPtr pAllPartInstances,
		LoopInstanceListPtr pLoopInsts_4_flyCut, bool bApplyTo_other_sameParts);
	~FlyCutSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartInstanceListPtr m_pAllPartInsts;
	LoopInstanceListPtr m_pLoopInsts_4_flyCut;
	bool m_bApplyTo_other_sameParts; // 是否应用到其他相同零件。

	// do产生的中间数据，undo/redo时要用到。
	std::vector<SequenceReplaceInfo> m_sequenceReplaceData;
};

END_CUTLEADER_NAMESPACE()
