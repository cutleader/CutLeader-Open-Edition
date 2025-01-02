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

	// 根据轮廓尺寸得到合适的刀具分配规则。内外轮廓的规则不会混在一起，所以这里不需传类似参数。
	ToolAssignRulePtr GetToolAssignRule_byLoopSize(double dLoopWidth, double dLoopHeight);

	// 根据刀具得到规则
	ToolAssignRuleListPtr GetRules_byTool(ToolInstancePtr pToolInstance) const;
};

END_CUTLEADER_NAMESPACE()
