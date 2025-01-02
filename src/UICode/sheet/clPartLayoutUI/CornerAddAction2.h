#pragma once

#include "PartPlacementEditAction.h"
#include "CornerConfigItem.h"
#include "SpreadSamePartActionBar.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 在板材模式下增加角特征的动作。
class CornerAddAction2 : public PartPlacementEditAction
{
	typedef enum tagMicroJointActionState
	{
		CORNER_STATE_NONE				= 0,
		CORNER_STATE_SELECTCORNER		= 1,
	} CORNER_ACTION_STATE;

public:
	CornerAddAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~CornerAddAction2(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return PMT_CORNER_ADD; }
	virtual int GetActionName() const override { return IDS_AddCornerFeature; }

private:
	CORNER_ACTION_STATE m_state;

	// 当前的回路实例。
	LoopInstancePtr m_pCurLoopInst;

	// 当前的角点。
	Point2DPtr m_pCurCornerPt;

	// 缓存所有的回路实例。
	LoopInstanceListPtr m_pAllLoopInstanceList;

	// “传播相同零件”对话框。
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()
