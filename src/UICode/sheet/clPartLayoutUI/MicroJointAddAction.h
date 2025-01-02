#pragma once

#include "clCutFeatureCommon.h"
#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 给回路添加“微连接”特征。
class MicroJointAddAction : public PartPlacementEditAction
{
public:
	MicroJointAddAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
        PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, ParamConfigPtr pParamConfig, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~MicroJointAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return PMT_MJ_ADD; }
	virtual int GetActionName() const override { return IDS_AddMicroJoint; }
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 当前被捕获的回路实例。
	LoopInstancePtr m_pActiveLoopInstance;

	LoopInstanceListPtr m_pAllLoopInstances;
	ParamConfigPtr m_pParamConfig;
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()
