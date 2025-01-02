#pragma once

#include "Point2D.h"
#include "CutFeatureEditBase.h"

DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeatureUI_Export LoopToolEditAction : public CutFeatureEditBase
{
    enum PATLOOP_SELECT_STATE
    {
        NONE						    = 0,
        PATLOOP_SELECT_FIRST_POINT		= 1, // 左键按下
        PATLOOP_SELECT_SECOND_POINT		= 2, // 左键抬起
    };

public:
	LoopToolEditAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort,
        ToolAssignConfigItemPtr pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList);
	~LoopToolEditAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
    virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return LOOPTOOL_EDIT; }
	virtual int GetActionName() const override { return IDS_COMMAMD_CHANGE_TOOL; }
	virtual void UpdateForUndoRedo() override;

private:
    ToolAssignConfigItemPtr m_pToolAssignConfigItem;
	LeadConfigItemPtr m_pLeadConfigItem;
    ConditionTypeListPtr m_pCondTypeList;

    PATLOOP_SELECT_STATE m_iState;

    // 框选的框框(左上x，左上y，右下x，右下y)
	double m_boxData[4];

	// 鼠标移动时的候选轮廓，只有一个有效。
	IPatternLoopPtr m_pActivePatternLoop; // 不框选时的“当前轮廓”。
	PatternLoopListPtr m_pActivePatternLoops_inRect; // 框选时的“当前轮廓”。

    // 选中的轮廓，左键抬起时更新
    PatternLoopListPtr m_pSelectedPatternLoops;
};

END_CUTLEADER_NAMESPACE()
