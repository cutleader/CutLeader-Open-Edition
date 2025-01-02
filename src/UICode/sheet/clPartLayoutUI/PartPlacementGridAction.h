#pragma once

#include "PartPlacementPreSelectAction.h"
#include "ObserverBase.h"

DECLARE_CUTLEADER_CLASS(CreatePartGridActionBar)
DECLARE_CUTLEADER_CLASS(PartPlacement)

BEGIN_CUTLEADER_NAMESPACE()

// 这个动作创建零件阵列。
class PartPlacementGridAction : public PartPlacementPreSelectAction, public ObserverBase
{
public:
	// 集合pSelectedPartPlacements中只传入一个非阵列零件放置，或者集合中为空。
	PartPlacementGridAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort, CreatePartGridActionBar* pCreatePartGridActionBar);
	~PartPlacementGridAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_GRIDPRTPMT; }
	virtual int GetActionName() const override { return IDS_COMMAND_GRID_PRTPMT; }
	virtual void UpdateForUndoRedo() override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

private:
	void GeneratePartPlacement4Preview();

private:
	CreatePartGridActionBar* m_pCreatePartGridActionBar;

	// 创建出来的阵列零件放置，只要参数发生变化，即更新该对象。
	PartPlacementPtr m_pPartPlacement4Preview;
};

END_CUTLEADER_NAMESPACE()
