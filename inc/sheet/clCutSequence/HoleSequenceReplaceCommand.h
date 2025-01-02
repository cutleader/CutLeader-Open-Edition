#pragma once

#include "SequenceCommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export HoleSequenceReplaceCommand : public SequenceCommandBase
{
	// "�滻��Ϣ"
	struct Seq_replace_info
	{
		CutSequenceListPtr m_pNewSeqs; // �µĹ���
		std::map<int, ICutSequencePtr> m_pOldCutSequences; // �ɵĹ���
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

	// Դ��Ŀ�ꡣ
	PartInstancePtr m_pPartInst_source; // �������ڲ�������һЩ�������й��򣬲�����Щ�����������ģ�Ҳ����˵��Щ����֮��û����������Ĺ���
	PartInstanceListPtr m_pPartInsts_target; // ��Щ����ڿ׿���û�й���û�й���Ĳ��ᱻ�滻��

	// do�������м����ݣ�undo/redoʱҪ�õ���
	std::vector<Seq_replace_info> m_seqReplaceData;
};

END_CUTLEADER_NAMESPACE()
