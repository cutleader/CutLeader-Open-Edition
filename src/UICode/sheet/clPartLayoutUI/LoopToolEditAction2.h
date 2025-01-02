#pragma once

#include "Point2D.h"
#include "ObserverBase.h"
#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"
#include "WrongToolSizeLoopInstances.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class LoopToolEditAction2 : public PartPlacementEditAction, public ObserverBase
{
    enum LOOPINST_SELECT_STATE
    {
        NONE						    = 0,
        LOOPINST_SELECT_FIRST_POINT		= 1, // 左键按下
        LOOPINST_SELECT_SECOND_POINT	= 2, // 左键抬起
    };

public:
	LoopToolEditAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort,
        ToolAssignConfigItemPtr pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~LoopToolEditAction2(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
    virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return LOOPTOOL_ADJUST; }
	virtual int GetActionName() const override { return IDS_COMMAMD_CHANGE_TOOL; }
	virtual void UpdateForUndoRedo() override;

public: // implement IObserver interface.
    virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
    void EditToolsOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances);

private:
    void DisplayLoopToolInfo();

private:
    ToolAssignConfigItemPtr m_pToolAssignConfigItem;
	LeadConfigItemPtr m_pLeadConfigItem;
    ConditionTypeListPtr m_pCondTypeList;

    LOOPINST_SELECT_STATE m_iState;

    // 框选的框框(左上x，左上y，右下x，右下y)
	double m_boxData[4];

	// 鼠标移动时的候选轮廓，只有一个有效。
	LoopInstancePtr m_pActiveLoopInstance; // 不框选时的“当前轮廓”。
	LoopInstanceListPtr m_pActiveLoopInsts_inRect; // 框选时的“当前轮廓”。

    // 选中的轮廓，左键抬起时更新
    LoopInstanceListPtr m_pSelectedLoopInsts;

	// 缓存所有的“回路实例”。
	LoopInstanceListPtr m_pAllLoopInstances;

    // “传播相同零件”对话框。
    SpreadSamePartActionBar* m_pSpreadSamePartActionBar;

    // 当每次设置轮廓的刀具尺寸时，把偏移有问题的轮廓记录在这里。
    // 注：
    //  1) 在该action下，每设置一次刀具尺寸都要更新这个集合。离开了这个action该信息就被清除了。
    WrongToolSizeLoopInstancesPtr m_pWrongToolSizeLoopInstances;
};

END_CUTLEADER_NAMESPACE()
