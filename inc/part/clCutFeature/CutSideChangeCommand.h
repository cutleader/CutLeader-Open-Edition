#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// ���������޸ļ��������������з�ʽ��
// ע��
// 1) ����ļ�������Ҫ��ȷ���Ա��޸ĳ�emCutOffsetType��ʽ�������޸ĵ�����Ҫ���˵���
//    �磺emCutOffsetType�����У���ôm_pLoopStartCutDataList�в�Ҫ�����е�������
class ClCutFeature_Export CutSideChangeCommand : public CommandBase
{
public:
    CutSideChangeCommand(const EditorData& editorData, LoopStartCutDataListPtr pLoopStartCutDataList, PatternLoopCutOffsetType emCutOffsetType,
		LeadConfigItemPtr pLeadConfigItem);
	~CutSideChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopStartCutDataListPtr m_pLoopStartCutDataList;
	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;

	// ��¼֮ǰ��cut side��������Ϣ������undo
	std::map<LONGLONG, PatternLoopCutSideType> m_oldCutsideData;
	std::map<LONGLONG, ILeadPtr> m_oldLeadInData;
	std::map<LONGLONG, ILeadPtr> m_oldLeadOutData;
};

END_CUTLEADER_NAMESPACE()
