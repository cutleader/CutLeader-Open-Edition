#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>
#include <set>

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ToolInstance)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ���ڱ����ϵĵ��ߵ���Ϣ
struct LoopTool_loopLead_Data_2
{
    bool m_bIsToolDiabled;

	// ������Ϣ
	CString m_strToolName;
	double m_dToolWidth;
	COLORREF m_iToolColor;
	ConditionNodeListPtr m_pCondNodeList;

	// ƫ�Ʒ����������Ϣ
	PatternLoopCutSideType m_iCutside;
	ILeadPtr m_pLeadIn;
	ILeadPtr m_pLeadOut;
};

// �޸��������ߵĳߴ硣
// ע��
// 1����LoopToolChangeCommand�������ǣ����������doʱ�������ϵĵ�����Ϊ�����á���
class ClCutFeature_Export LoopToolChangeCommand_2 : public CommandBase
{
public:
	LoopToolChangeCommand_2(EditorData editorData, const std::vector<LoopTopologyItemListPtr>& loopTopologyItemData, LoopToolDataListPtr pLoopToolList,
		ToolInstancePtr pToolInstance_new, LoopStartCutDataListPtr pLoopStaEndCutList, LeadConfigItemPtr pLeadConfigItem);
	~LoopToolChangeCommand_2(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	const std::vector<LoopTopologyItemListPtr> m_loopTopologyItemData;
	LoopToolDataListPtr m_pLoopToolList;
	LoopStartCutDataListPtr m_pLoopStaEndCutList;
	LeadConfigItemPtr m_pLeadConfigItem;

	// �ϵĵ�����Ϣ��
	std::vector<LoopTool_loopLead_Data_2> m_oldTool_lead_Datas;

	// �µĵ�����Ϣ��
	ToolInstancePtr m_pToolInstance_new;

	// ��¼һ����Щ���������������и�ƫ�Ʒ�ʽ�����ߵ�
	std::set<LONGLONG> m_loopStaEndCutList_reAssignCutSideAndLead;
};

END_CUTLEADER_NAMESPACE()
