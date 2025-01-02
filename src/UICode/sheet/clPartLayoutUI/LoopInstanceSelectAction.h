#pragma once

#include "Point2D.h"
#include "ObserverBase.h"
#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"
#include "SelectPartOrLoopActionBar.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceSelectAction)

BEGIN_CUTLEADER_NAMESPACE()

class LoopInstanceSelectAction : public PartPlacementEditAction
{
    enum LOOPINST_SELECT_STATE
    {
        NONE						    = 0,
        LOOPINST_SELECT_FIRST_POINT		= 1, // 左键按下
        LOOPINST_SELECT_SECOND_POINT	= 2, // 左键抬起
    };

public:
	LoopInstanceSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
        GlViewPortPtr pViewPort, SelectPartOrLoopActionBar* pSelectPartOrLoopActionBar);
	~LoopInstanceSelectAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
    virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_SelectLoopInstance; }
	virtual int GetActionName() const override { return IDS_SelectLoopInstance; }

public:
    LoopInstanceListPtr GetSelectedLoopInstances() const { return m_pSelectedLoopInsts; }

	// 清空选择集，选择轮廓实例。
	void SelectLoopInstances(LoopInstanceListPtr pSelectedLoopInsts);

private:
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

    SelectPartOrLoopActionBar* m_pSelectPartOrLoopActionBar;
};

END_CUTLEADER_NAMESPACE()
