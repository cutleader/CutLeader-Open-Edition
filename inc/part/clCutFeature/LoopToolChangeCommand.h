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

// 用于备份老的刀具等信息
struct LoopTool_loopLead_Data
{
	// 刀具信息
	CString m_strToolName;
	double m_dToolWidth;
	COLORREF m_iToolColor;
	ConditionNodeListPtr m_pCondNodeList;

	// 偏移方向和引线信息
	PatternLoopCutSideType m_iCutside;
	ILeadPtr m_pLeadIn;
	ILeadPtr m_pLeadOut;
};

// 修改轮廓刀具的尺寸。
class ClCutFeature_Export LoopToolChangeCommand : public CommandBase
{
public:
	LoopToolChangeCommand(EditorData editorData, const std::vector<LoopTopologyItemListPtr>& loopTopologyItemData, LoopToolDataListPtr pLoopToolList,
		ToolInstancePtr pToolInstance_new, LoopStartCutDataListPtr pLoopStaEndCutList, LeadConfigItemPtr pLeadConfigItem);
	~LoopToolChangeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	const std::vector<LoopTopologyItemListPtr> m_loopTopologyItemData;
	LoopToolDataListPtr m_pLoopToolList;
	LoopStartCutDataListPtr m_pLoopStaEndCutList;
	LeadConfigItemPtr m_pLeadConfigItem;

	// 老的刀具信息。
	std::vector<LoopTool_loopLead_Data> m_oldTool_lead_Datas;

	// 新的刀具信息。
	ToolInstancePtr m_pToolInstance_new;

	// 记录一下哪些轮廓重新设置了切割偏移方式和引线的
	std::set<LONGLONG> m_loopStaEndCutList_reAssignCutSideAndLead;
};

END_CUTLEADER_NAMESPACE()
