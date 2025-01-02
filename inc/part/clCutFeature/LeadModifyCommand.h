#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LeadList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸ġ�����������ߡ������
// ע��
//   1) �޸ĵķ�ʽ���滻���ϵġ����������ߡ���
class ClCutFeature_Export LeadModifyCommand : public CommandBase
{
public:
	LeadModifyCommand(EditorData editorData, LeadInOutType iLeadType, LoopStartCutDataListPtr pLoopStaEndCutList, LeadListPtr pLeadList);
	~LeadModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ���޸������߻��������ߡ�
	LeadInOutType m_iLeadType;

	// Ҫ�޸ĵġ���·���С�������
	LoopStartCutDataListPtr m_pLoopStaEndCutList;

	// �µ�����������߼��ϣ������ÿ��Ԫ�ض���ͬ��
	LeadListPtr m_pLeadList;

	// �����ϵ�����������߼��ϡ�
	LeadListPtr m_pOldLeadList;
};

END_CUTLEADER_NAMESPACE()
