#pragma once

#include "Point2D.h"
#include "ObserverBase.h"
#include "PartPlacementEditAction.h"
#include "ChangeLoopToolActionBar.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopToolEditAction3)


BEGIN_CUTLEADER_NAMESPACE()

// 通过点击色块来编辑轮廓的刀具。
class ClPartLayoutUI_Export LoopToolEditAction3 : public PartPlacementEditAction
{
    enum LOOPINST_SELECT_STATE
    {
        NONE						    = 0,
        LOOPINST_SELECT_FIRST_POINT		= 1, // 左键按下
        LOOPINST_SELECT_SECOND_POINT	= 2, // 左键抬起
    };

public:
	LoopToolEditAction3(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		int iToolIndex, GlViewPortPtr pViewPort, ToolAssignConfigItemPtr pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList,
		ChangeLoopToolActionBar* pChangeLoopToolActionBar);
	~LoopToolEditAction3(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
    virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return LOOPTOOL_ADJUST_2; }
	virtual int GetActionName() const override { return IDS_COMMAMD_CHANGE_TOOL; }
	virtual void UpdateForUndoRedo() override;

public:
    void SetTool_toSelectedLoopInstances(LoopInstanceListPtr pLoopInstances);

private:
    ToolAssignConfigItemPtr m_pToolAssignConfigItem;
	LeadConfigItemPtr m_pLeadConfigItem;
    ConditionTypeListPtr m_pCondTypeList;

    LOOPINST_SELECT_STATE m_iState;

    // 框选的框框(左上x，左上y，右下x，右下y)
	double m_boxData[4];

	// 鼠标移动时的候选轮廓。
	LoopInstanceListPtr m_pActiveLoopInsts;

	// 缓存所有的“回路实例”。
	LoopInstanceListPtr m_pAllLoopInstances;

    // “传播相同零件”对话框。
    ChangeLoopToolActionBar* m_pChangeLoopToolActionBar;

    int m_iToolIndex;
};

END_CUTLEADER_NAMESPACE()
