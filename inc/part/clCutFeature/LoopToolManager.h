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

// ����·������������������
class ClCutFeature_Export LoopToolManager
{
public:
    static void AssignLoopTool(LoopToolDataPtr pLoopTool, const LoopTopologyItemList* pLoopTopologyItems, const ToolAssignConfigItem* pToolAssignConfigItem);

    // ���ݡ��������͡����������ڵ㣬ֵ�á���ʾ���͵ġ���
    static ConditionNodePtr BuildCondNode(ConditionTypeListPtr pCondTypeList, LONGLONG iCondTypeID, CString strCondVal_4_display);

	// �õ�������ʾ��ֵ
	static CString GetConditionValue_4_display(ConditionNodePtr pCondNode, ConditionTypeListPtr pCondTypeList);

    // �õ������Ļ�����ɫ���õ�����ɫ������������ӹ��������ض�����ɫ��
    static COLORREF GetLoopDisplayColor(const LoopToolData* pLoopToolData);
};

END_CUTLEADER_NAMESPACE()
