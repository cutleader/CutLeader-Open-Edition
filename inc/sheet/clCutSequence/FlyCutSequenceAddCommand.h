#pragma once

#include "SequenceCommandBase.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// �����������и�򡱵����
// ע��
// 1) �����Щ�������Ѿ����˹�����ôɾ����Щ���򣬲��ѷ��й������֮ǰ�Ĺ���λ�á������Щ�����ϻ�û�й�����ô�ѷ��й�����ڹ��򼯺ϵ����
class ClCutSequence_Export FlyCutSequenceAddCommand : public SequenceCommandBase
{
	// "�滻��Ϣ"
	struct SequenceReplaceInfo
	{
		ICutSequencePtr m_pNewFlyCutSeq; // �µķ����и��
		std::map<int, ICutSequencePtr> m_oldCutSequences; // �ɵĹ��򣬼����п���Ϊ�ա�
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
	bool m_bApplyTo_other_sameParts; // �Ƿ�Ӧ�õ�������ͬ�����

	// do�������м����ݣ�undo/redoʱҪ�õ���
	std::vector<SequenceReplaceInfo> m_sequenceReplaceData;
};

END_CUTLEADER_NAMESPACE()
