#pragma once

#include "SequenceCommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export HoleSequenceReplaceCommand : public SequenceCommandBase
{
	// "替换信息"
	struct Seq_replace_info
	{
		CutSequenceListPtr m_pNewSeqs; // 新的工序
		std::map<int, ICutSequencePtr> m_pOldCutSequences; // 旧的工序
	};

public:
	HoleSequenceReplaceCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartInstanceListPtr pAllPartInstances,
					  PartInstancePtr pPartInst_source, PartInstanceListPtr pPartInsts_target);
	~HoleSequenceReplaceCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartInstanceListPtr m_pAllPartInsts;

	// 源和目标。
	PartInstancePtr m_pPartInst_source; // 这个零件内部必须有一些孔上面有工序，并且这些工序是连续的，也就是说这些工序之间没有其他零件的工序。
	PartInstanceListPtr m_pPartInsts_target; // 这些零件内孔可以没有工序，没有工序的不会被替换。

	// do产生的中间数据，undo/redo时要用到。
	std::vector<Seq_replace_info> m_seqReplaceData;
};

END_CUTLEADER_NAMESPACE()
