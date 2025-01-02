#pragma once

#include "clCutFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(ConditionType)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ConditionNode)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// “回路刀具特征”管理器。
class ClCutFeature_Export LoopToolManager
{
public:
    static void AssignLoopTool(LoopToolDataPtr pLoopTool, const LoopTopologyItemList* pLoopTopologyItems, const ToolAssignConfigItem* pToolAssignConfigItem);

    // 根据“条件类型”创建条件节点，值用“显示类型的”。
    static ConditionNodePtr BuildCondNode(ConditionTypeListPtr pCondTypeList, LONGLONG iCondTypeID, CString strCondVal_4_display);

	// 得到用于显示的值
	static CString GetConditionValue_4_display(ConditionNodePtr pCondNode, ConditionTypeListPtr pCondTypeList);

    // 得到轮廓的绘制颜色，用刀具颜色，如果轮廓不加工，就用特定的颜色。
    static COLORREF GetLoopDisplayColor(const LoopToolData* pLoopToolData);
};

END_CUTLEADER_NAMESPACE()
