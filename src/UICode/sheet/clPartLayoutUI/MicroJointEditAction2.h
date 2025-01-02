#pragma once

#include "ObserverBase.h"
#include "MicroJointPreSelectAction2.h"
#include "SpreadSamePartActionBar.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// “编辑微连接”动作。
class MicroJointEditAction2 : public MicroJointPreSelectAction2, public ObserverBase
{
public:
	MicroJointEditAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~MicroJointEditAction2(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return PMT_MJ_EDIT; }
	virtual int GetActionName() const override { return IDS_EditMicroJoint; }
	virtual void UpdateForUndoRedo() override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

private:
	void DisplayMJData();

private:
	/************************************************************************/
	// 用来改变回路实例的起切点位置。

	// 选中的回路实例。
	LoopInstancePtr m_pSelLoopInst;

	// 新的微连接位置。
	Point2D m_newPt;
	int m_iPatIndex;
	/************************************************************************/

	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()
