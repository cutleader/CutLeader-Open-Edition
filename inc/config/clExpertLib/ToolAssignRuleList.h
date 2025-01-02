#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ToolInstance)
DECLARE_CUTLEADER_CLASS(ToolAssignRule)
DECLARE_CUTLEADER_CLASS(ToolAssignRuleList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ToolAssignRuleList : public std::vector<ToolAssignRulePtr>,
                                            public ClData
{
public:
	ToolAssignRuleList();
	~ToolAssignRuleList();

public: // implements IData interface.
    virtual BOOL IsModified() const override;

public:
    ToolAssignRulePtr GetToolAssignRule_byID(LONGLONG iToolAssignRuleID) const;
    void DeleteToolAssignRule_byID(LONGLONG iToolAssignRuleID);

	// ���������ߴ�õ����ʵĵ��߷���������������Ĺ��򲻻����һ���������ﲻ�贫���Ʋ�����
	ToolAssignRulePtr GetToolAssignRule_byLoopSize(double dLoopWidth, double dLoopHeight);

	// ���ݵ��ߵõ�����
	ToolAssignRuleListPtr GetRules_byTool(ToolInstancePtr pToolInstance) const;
};

END_CUTLEADER_NAMESPACE()
