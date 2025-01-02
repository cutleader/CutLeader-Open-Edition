#pragma once

#include "ClData.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ToolInstance)
DECLARE_CUTLEADER_CLASS(ToolInstanceList)
DECLARE_CUTLEADER_CLASS(ToolAssignRuleList)
DECLARE_CUTLEADER_CLASS(DxfLayerToolMap)

BEGIN_CUTLEADER_NAMESPACE()

enum DxfLayerToolMapType
{
	DxfLayerToolMap_notUse		= 0,
	DxfLayerToolMap_layer		= 1,
	DxfLayerToolMap_color		= 2
};

// ���߷������
class ClExpertLib_Export ToolAssignConfigItem : public ClData,
								   virtual public ParamConfigItem
{
public:
	ToolAssignConfigItem(ToolInstanceListPtr pToolInstanceList, ToolInstancePtr pDefaultTool_4_outerLoop, ToolAssignRuleListPtr pToolAssignRules_4_outerLoop,
                         ToolInstancePtr pDefaultTool_4_innerLoop, ToolAssignRuleListPtr pToolAssignRules_4_innerLoop,
						 DxfLayerToolMapType iDxfLayerToolMapType, DxfLayerToolMapPtr pDxfLayerToolMap);
	virtual ~ToolAssignConfigItem(void);

public: // implements IData interface.
    virtual BOOL IsModified() const override;
	virtual IDataPtr Clone() const override; // ����parent id

public: // implement interface ParamConfigItem.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_TOOL_ASSIGN; }

public: // get/set functions.
	ToolInstanceListPtr GetToolInstanceList() const { return m_pToolInstanceList; }

    ToolInstancePtr GetDefaultTool_4_outerLoop() const { return m_pDefaultTool_4_outerLoop; }
    void SetDefaultTool_4_outerLoop(ToolInstancePtr pDefaultTool_4_outerLoop) { m_pDefaultTool_4_outerLoop = pDefaultTool_4_outerLoop; }

    ToolAssignRuleListPtr GetToolAssignRules_4_outerLoop() const { return m_pToolAssignRules_4_outerLoop; }

    ToolInstancePtr GetDefaultTool_4_innerLoop() const { return m_pDefaultTool_4_innerLoop; }
    void SetDefaultTool_4_innerLoop(ToolInstancePtr pDefaultTool_4_innerLoop) { m_pDefaultTool_4_innerLoop = pDefaultTool_4_innerLoop; }

    ToolAssignRuleListPtr GetToolAssignRules_4_innerLoop() const { return m_pToolAssignRules_4_innerLoop; }

	DxfLayerToolMapType GetDxfLayerToolMapType() const { return m_iDxfLayerToolMapType; }
	void SetDxfLayerToolMapType(DxfLayerToolMapType iDxfLayerToolMapType) { m_iDxfLayerToolMapType = iDxfLayerToolMapType; }

	DxfLayerToolMapPtr GetDxfLayerToolMap() const { return m_pDxfLayerToolMap; }
	void SetDxfLayerToolMap(DxfLayerToolMapPtr pDxfLayerToolMap) { m_pDxfLayerToolMap = pDxfLayerToolMap; }

private:
    // ���е���ʵ��
    ToolInstanceListPtr m_pToolInstanceList;

    // ���߷������
    ToolInstancePtr m_pDefaultTool_4_outerLoop; // ��������Ĭ�ϵ���
    ToolAssignRuleListPtr m_pToolAssignRules_4_outerLoop; // �������ĵ��߹���
    ToolInstancePtr m_pDefaultTool_4_innerLoop; // ��������Ĭ�ϵ���
    ToolAssignRuleListPtr m_pToolAssignRules_4_innerLoop; // �������ĵ��߹���

	// dxf����ʱͼ����ɫ�͵��ߵ�ӳ��
	DxfLayerToolMapType m_iDxfLayerToolMapType;
	DxfLayerToolMapPtr m_pDxfLayerToolMap;
};

END_CUTLEADER_NAMESPACE()
