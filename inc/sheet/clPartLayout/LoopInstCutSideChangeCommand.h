#pragma once

#include <map>
#include "clCutFeatureCommon.h"
#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// ���������޸�����ʵ���������з�ʽ��
// ע��
// 1) ���������ʵ��Ҫ��ȷ���Ա��޸ĳ�emCutOffsetType��ʽ�������޸ĵ�����Ҫ���˵���
//    �磺emCutOffsetType�����У���ôm_pLoopInstances�в�Ҫ�����е�������
class ClPartLayout_Export LoopInstCutSideChangeCommand : public CommandBase
{
	typedef std::pair<LONGLONG, LONGLONG> IdPair;

public:
	LoopInstCutSideChangeCommand(const EditorData& editorData, LoopInstanceListPtr pLoopInstances, PatternLoopCutOffsetType emCutOffsetType,
		LeadConfigItemPtr pLeadConfigItem);
	~LoopInstCutSideChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopInstanceListPtr m_pLoopInstances;
	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;

	// ��¼֮ǰ��cut side��������Ϣ������undo��IdPairΪ<���ʵ��ID����������ID>��
	std::map<IdPair, PatternLoopCutSideType> m_oldCutsideData;
	std::map<IdPair, ILeadPtr> m_oldLeadInData;
	std::map<IdPair, ILeadPtr> m_oldLeadOutData;
};

END_CUTLEADER_NAMESPACE()
